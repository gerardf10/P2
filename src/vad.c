#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pav_analysis.h"
#include "vad.h"

const float FRAME_TIME = 10.0F; /* Duración del frame en ms. */

/* Etiquetas para los estados */
const char *state_str[] = {
  "UNDEF", "S", "V", "INIT"
};

const char *state2str(VAD_STATE st) {
  return state_str[st];
}

/* Estructura para almacenar características del frame */
typedef struct {
  float zcr;
  float p;   /* potencia en dB (compute_power devuelve dB) */
  float am;
} Features;

/* Calcula las características del frame */
Features compute_features(const float *x, int N, int fm) {
  Features feat;
  feat.p = compute_power(x, N);
  feat.zcr = compute_zcr(x, N, fm);
  feat.am = compute_am(x, N);
  return feat;
}

/* Inicialización del VAD */
VAD_DATA *vad_open(float rate) {
  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = (unsigned int)(rate * FRAME_TIME * 1e-3);

  /* Inicialización de parámetros adaptativos */
  vad_data->noise_sum = 0.0;
  vad_data->noise_zcr_sum = 0.0;  // Nuevo campo
  vad_data->init_count = 0;
  vad_data->noise_level = -100.0;  /* Valor inicial muy bajo */
  vad_data->k_voice = -40.0;  
  vad_data->k_silence = -50.0;
  
  vad_data->last_feature = 0.0;
  vad_data->p0 = 5;  /* Valor por defecto de α, ya se cambia luego */
  vad_data->count_voice = 0;
  vad_data->count_silence = 0;  // Inicialización del hangover
  vad_data->voice_segment_count = 0;
  vad_data->total_voice_frames = 0;
  vad_data->max_silence_in_voice = 0;
  vad_data->adaptive_hangover = 5;  // Valor inicial
    // In vad_open function
  vad_data->max_power = 0.0;       // Start with reasonable values
  vad_data->min_power = -100.0; 
  vad_data->max_power_real = -60.0;       // Start with reasonable values
  vad_data->min_power_real = -30.0; 
  return vad_data;
}

/* Devuelve el tamaño del frame */
unsigned int vad_frame_size(VAD_DATA *vad_data) {
  return vad_data->frame_length;
}

/* Función principal de detección.
   - Durante ST_INIT se acumulan 10 frames para estimar el ruido.
   - Al finalizar se calculan:
       k_voice = noise_level + 10.0 * alpha0
       k_silence = noise_level + 3.0 * alpha0
   - Luego se alterna entre ST_SILENCE y ST_VOICE según la potencia.
   Durante INIT se retorna ST_SILENCE para evitar que INIT se emita en el .vad.
*/
VAD_STATE vad(VAD_DATA *vad_data, float *x, float alpha0) {
  Features f = compute_features(x, vad_data->frame_length, vad_data->sampling_rate);
  vad_data->last_feature = f.p;  /* Guardar potencia para depuración */
  // In vad function, after computing features
  // Update min/max power values (with protection against extreme outliers)
  if (f.p < vad_data->min_power) vad_data->min_power = f.p;
  if (f.p > vad_data->max_power) vad_data->max_power = f.p;

  // Calculate normalized power
  float f_norm = 0.0;
  if (vad_data->max_power > vad_data->min_power) {
      f_norm = (f.p - vad_data->min_power) / (vad_data->max_power - vad_data->min_power);
  }
  if (f.p < vad_data->min_power_real) vad_data->min_power_real = f.p;
  if (f.p > vad_data->max_power_real) vad_data->max_power_real = f.p;

  // Calculate normalized power
  float f_norm_real = 0.0;
  if (vad_data->max_power_real > vad_data->min_power_real) {
      f_norm_real = (f.p - vad_data->min_power_real) / (vad_data->max_power_real - vad_data->min_power_real);
  }
  switch (vad_data->state) {
    case ST_INIT:
      vad_data->noise_sum += f.p;
      vad_data->noise_zcr_sum += f.zcr;
      vad_data->init_count++;

      if (vad_data->init_count >= 10) {  // Usamos los primeros 10 frames
        vad_data->noise_level = vad_data->noise_sum / vad_data->init_count;
        vad_data->noise_zcr = vad_data->noise_zcr_sum / vad_data->init_count;
        /* Calculamos umbrales con pesos diferenciados para cada estado */
        vad_data->k_voice = vad_data->noise_level + 2.41 * vad_data->p0;
        vad_data->k_silence = vad_data->noise_level + 0.985 *vad_data->p0; ;
        vad_data->state = ST_SILENCE;
        if (f.p > vad_data->k_voice && f.zcr > 0.015) {
          vad_data->state = ST_VOICE;
        }
      }
      if(f.p > -10){
        return ST_VOICE;
      }
      // Aquí no debes retornar directamente - deja que continúe hasta el final
      if (vad_data->state == ST_INIT) {
        return ST_SILENCE;
      }
      break; // Now this break will execute
      case ST_SILENCE:
      // Se requiere que la potencia sea alta Y que la ZCR sea mayor que un umbral
      // In ST_SILENCE case:
      if (f.p > vad_data->k_voice - 0.95 && f.zcr > 0.088) {
        vad_data->count_voice++;
        if (vad_data->count_voice >= 1) {  //Esta en 1, es decir no hangover, si pongo no va.
          vad_data->state = ST_VOICE;
          vad_data->count_voice = 0;
          printf("silence to voice: %f %f\n", f_norm, f_norm_real);
        }
      } else {
        vad_data->count_voice = 0;
      }

        break;
      case ST_VOICE:
      /* Ajuste para mejor balance entre recall y precisión */
      if (f.p < vad_data->k_silence || (f.p < vad_data->k_silence + 0.95 && f.zcr < 0.048)) {
        vad_data->count_silence++;
        if (vad_data->count_silence >= 7) {  
          vad_data->state = ST_SILENCE;
          vad_data->count_silence = 0;
          printf("voice to silence: %f %f\n", f_norm, f_norm_real);
        }
      } else {
        vad_data->count_silence = 0;
      }
       // For voice-to-silence transitions with high confidence
        if (f_norm < 0.45) {  // Very low normalized power
          // Faster transition with less hangover
          if (vad_data->count_silence >= 5) {  // Reduced hangover
            vad_data->state = ST_SILENCE;
            vad_data->count_silence = 0;
            printf("voice to silence f_norm 0.45: %f %f\n", f_norm, f_norm_real);
          }
        }

        if (f_norm < 0.25) {  // Very low normalized power
          // Faster transition with less hangover
          if (vad_data->count_silence >= 3) {  // Reduced hangover
            vad_data->state = ST_SILENCE;
            vad_data->count_silence = 0;
            printf("voice to silence f_norm 0.25: %f %f\n", f_norm, f_norm_real);
          }
        }
        if (f_norm < 0.2) {  // Very low normalized power
          // Faster transition with less hangover
          if (vad_data->count_silence >= 1) {  // Reduced hangover
            vad_data->state = ST_SILENCE;
            vad_data->count_silence = 0;
            printf("voice to silence f_norm 0.2: %f %f\n", f_norm, f_norm_real);
          }
        }

      break;
    case ST_UNDEF:
    break;

    }

// Add state stability logic to prevent rapid transitions
/*static VAD_STATE last_state = ST_SILENCE;
static int state_duration = 0;
static int min_state_duration = 3; // Minimum of 3 frames (150ms) in each state

// Check if state is about to change
if (last_state != vad_data->state && vad_data->init_count >= 10) {
  // If we've been in the previous state for less than min_state_duration frames
  if (state_duration < min_state_duration) {
    // Don't allow the state change
    vad_data->state = last_state;
    printf("preventing rapid transition, maintaining state: %s\n", state2str(last_state));
  } else {
    // Reset duration counter for new state
    state_duration = 0;
  }
} else {
  // Same state continues, increment duration
  state_duration++;
}
// Update last state for next frame
last_state = vad_data->state;*/
/*
// Uncommented and optimized f_norm_real transitions
// For voice-to-silence acceleration using real power normalization
if (f_norm_real < 0.18) {  // Clear voice-to-silence indicator
  if (vad_data->count_silence >= 4) {
    vad_data->state = ST_SILENCE;
    vad_data->count_silence = 0;
    printf("voice to silence f_norm_real 0.18: %f %f\n", f_norm, f_norm_real);
  }
}

if (f_norm_real < 0.10) {  // Strong voice-to-silence indicator
  if (vad_data->count_silence >= 2) {
    vad_data->state = ST_SILENCE;
    vad_data->count_silence = 0;
    printf("voice to silence f_norm_real 0.10: %f %f\n", f_norm, f_norm_real);
  }
}

if (f_norm_real < 0.05) {  // Very clear voice-to-silence
  vad_data->state = ST_SILENCE;  // Immediate transition with no hangover
  vad_data->count_silence = 0;
  printf("voice to silence f_norm_real 0.05: %f %f\n", f_norm, f_norm_real);
}

// Combined metrics for improved silence detection
if (f_norm < 0.35 && f_norm_real < 0.25) {
  if (vad_data->count_silence >= 2) {
    vad_data->state = ST_SILENCE;
    vad_data->count_silence = 0;
    printf("voice to silence combined: %f %f\n", f_norm, f_norm_real);
  }
}

// ZCR-based voice-to-silence detection (low ZCR often indicates silence) - more sensitive
if (f.zcr < 0.038 && f_norm < 0.42) {  // Increased thresholds for better silence recall
  if (vad_data->count_silence >= 1) {  // Reduced hangover for faster transitions
    vad_data->state = ST_SILENCE;
    vad_data->count_silence = 0;
    printf("voice to silence low zcr: %f %f %f\n", f.zcr, f_norm, f_norm_real);
  }
}

// Adaptive threshold for transitional frames - more aggressive
if ((f_norm < 0.34 && f_norm_real < 0.24) || (f.p < vad_data->noise_level + 3.7)) { // Higher thresholds
  if (vad_data->count_silence >= 2) {  // Reduced hangover
    vad_data->state = ST_SILENCE;
    vad_data->count_silence = 0;
    printf("voice to silence adaptive: %f %f %f\n", f.p, f_norm, f_norm_real);
  }
}


// Special case for consistent low energy - more sensitive but with voice protection
if (vad_data->count_silence >= 4 && f.p < vad_data->noise_level + 4.0) {
  if (f.zcr < 0.06 || f_norm < 0.45) {  // Added voice protection check
    vad_data->state = ST_SILENCE;
    vad_data->count_silence = 0;
    printf("voice to silence persistent low energy: %f\n", f.p);
  }
}

// New condition for better silence detection in borderline cases - slightly more conservative
if (f.p < vad_data->noise_level + 2.8 && f.zcr < 0.048) {  // Reduced ZCR threshold
  if (vad_data->count_silence >= 2) {  // Increased hangover from 1 to 2
    vad_data->state = ST_SILENCE;
    vad_data->count_silence = 0;
    printf("voice to silence clear signature: %f %f\n", f.p, f.zcr);
  }
}

// Enhanced condition for detecting silence in speech pauses with voice protection
if (vad_data->state == ST_VOICE && f.p < vad_data->k_silence && f.zcr < vad_data->noise_zcr * 1.5) {
  if (f.zcr < 0.055 || f_norm < 0.40) {  // Added voice protection
    vad_data->state = ST_SILENCE;
    vad_data->count_silence = 0;
    printf("voice to silence in speech pause: %f %f\n", f.p, f.zcr);
  }
}

// Add voice protection - restore voice classification for clear voice segments
if (vad_data->state == ST_SILENCE && f.p > vad_data->k_voice && f.zcr > 0.07) {
  vad_data->state = ST_VOICE;
  printf("silence to voice protection: %f %f\n", f.p, f.zcr);
}

// Add protection for strong voice signals
if (vad_data->state == ST_SILENCE && f_norm > 0.55 && f.zcr > 0.065) {
  vad_data->state = ST_VOICE;
  printf("silence to voice strong signal: %f %f %f\n", f_norm, f_norm_real, f.zcr);
}*/

/*

// For voice-to-silence acceleration after your existing conditions:
if (f_norm_real < 0.15) {  // Clear voice-to-silence indicator
  if (vad_data->count_silence >= 4) {
    vad_data->state = ST_SILENCE;
    vad_data->count_silence = 0;
    printf("voice to silence: %f %f\n", f_norm, f_norm_real);
  }
}

if (f_norm_real < 0.08) {  // Strong voice-to-silence indicator
  if (vad_data->count_silence >= 2) {
    vad_data->state = ST_SILENCE;
    vad_data->count_silence = 0;
    printf("voice to silence: %f %f\n", f_norm, f_norm_real);
  }
}

if (f_norm_real < 0.04) {  // Very clear voice-to-silence
  if (vad_data->count_silence >= 1) {
    vad_data->state = ST_SILENCE;
    vad_data->count_silence = 0;
    printf("voice to silence f_norm_real: %f %f\n", f_norm, f_norm_real);
  }
}*/
  return vad_data->state;
}

/* Libera la memoria y devuelve el último estado */
VAD_STATE vad_close(VAD_DATA *vad_data) {
  VAD_STATE state = vad_data->state;
  free(vad_data);
  return state;
}

/* Muestra el estado actual para depuración */
void vad_show_state(const VAD_DATA *vad_data, FILE *out) {
  fprintf(out, "%s\t%f\n", state2str(vad_data->state), vad_data->last_feature);
}
