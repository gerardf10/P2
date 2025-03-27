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
        }
      } else {
        vad_data->count_silence = 0;
      }
      break;
    case ST_UNDEF:
    break;

    }
 


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
