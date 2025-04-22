#ifndef _VAD_H
#define _VAD_H
#include <stdio.h>
#define NOISE_UPDATE_RATE 0.1
/* Estados del VAD */
typedef enum {ST_UNDEF=0, ST_SILENCE, ST_VOICE, ST_INIT} VAD_STATE;

/* Retorna el label asociado a cada estado */
const char *state2str(VAD_STATE st);

/* Estructura que controla el VAD y almacena parámetros adaptativos */
typedef struct {
  VAD_STATE state;
  float sampling_rate;
  unsigned int frame_length;
  float last_feature;  /* para depuración, ej. potencia del frame */

  /* Variables para estimar el ruido adaptativamente */
  float noise_sum;     /* suma de potencias de frames en fase INIT */
  unsigned int init_count; /* número de frames usados para estimar el ruido */
  float noise_level;   /* nivel de ruido medio (en dB) */
  float k_voice;       /* umbral para detectar voz: noise_level + (10 * α) */
  float k_silence;     /* umbral para confirmar silencio: noise_level + (3 * α) */
  float noise_zcr;     /* nivel de zcr medio */   
  float noise_zcr_sum;
  float max_power;
  float min_power;
  float max_power_real;
  float min_power_real;
  float p0;            /* α, factor multiplicativo; por defecto 1.0 */

  unsigned int count_voice;   /* contador de frames que indican voz */
unsigned int count_silence; /* contador de frames que indican silencio */

unsigned int voice_segment_count;    // Número de segmentos de voz detectados
unsigned int total_voice_frames;     // Total de frames clasificados como voz
unsigned int max_silence_in_voice;   // Máxima duración de silencio encontrada en voz
unsigned int adaptive_hangover;      // Valor adaptativo de hangover


} VAD_DATA;

/* Inicializa el VAD y devuelve un puntero a la estructura */
VAD_DATA *vad_open(float sampling_rate);

/* Devuelve el tamaño del frame en muestras */
unsigned int vad_frame_size(VAD_DATA *);

/* Función principal: procesa un frame y devuelve el nuevo estado.
   Se asume que x tiene longitud frame_length.
   El parámetro alpha0 se usa como factor multiplicativo en los márgenes. */
VAD_STATE vad(VAD_DATA *vad_data, float *x, float alpha0);

/* Libera la memoria y devuelve el último estado */
VAD_STATE vad_close(VAD_DATA *vad_data);

/* Muestra el estado actual del VAD para depuración */
void vad_show_state(const VAD_DATA *, FILE *);

#endif
