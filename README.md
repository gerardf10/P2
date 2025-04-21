PAV - P2: detección de actividad vocal (VAD)
============================================

Guillem Moreno Garcia i Gerard Ferret Colomer.
---------------------------------------------


Esta práctica se distribuye a través del repositorio GitHub [Práctica 2](https://github.com/albino-pav/P2),
y una parte de su gestión se realizará mediante esta web de trabajo colaborativo.  Al contrario que Git,
GitHub se gestiona completamente desde un entorno gráfico bastante intuitivo. Además, está razonablemente
documentado, tanto internamente, mediante sus [Guías de GitHub](https://guides.github.com/), como
externamente, mediante infinidad de tutoriales, guías y vídeos disponibles gratuitamente en internet.


Inicialización del repositorio de la práctica.
----------------------------------------------

Para cargar los ficheros en su ordenador personal debe seguir los pasos siguientes:

*	Abra una cuenta GitHub para gestionar esta y el resto de prácticas del curso.
*	Cree un repositorio GitHub con el contenido inicial de la práctica (sólo debe hacerlo uno de los
	integrantes del grupo de laboratorio, cuya página GitHub actuará de repositorio central del grupo):
	-	Acceda la página de la [Práctica 2](https://github.com/albino-pav/P2).
	-	En la parte superior derecha encontrará el botón **`Fork`**. Apriételo y, después de unos segundos,
		se creará en su cuenta GitHub un proyecto con el mismo nombre (**P2**). Si ya tuviera uno con ese 
		nombre, se utilizará el nombre **P2-1**, y así sucesivamente.
*	Habilite al resto de miembros del grupo como *colaboradores* del proyecto; de este modo, podrán
	subir sus modificaciones al repositorio central:
	-	En la página principal del repositorio, en la pestaña **:gear:`Settings`**, escoja la opción 
		**Collaborators** y añada a su compañero de prácticas.
	-	Éste recibirá un email solicitándole confirmación. Una vez confirmado, tanto él como el
		propietario podrán gestionar el repositorio, por ejemplo: crear ramas en él o subir las
		modificaciones de su directorio local de trabajo al repositorio GitHub.
*	En la página principal del repositorio, localice el botón **Branch: master** y úselo para crear
	una rama nueva con los primeros apellidos de los integrantes del equipo de prácticas separados por
	guion (**fulano-mengano**).
*	Todos los miembros del grupo deben realizar su copia local en su ordenador personal.
	-	Copie la dirección de su copia del repositorio apretando en el botón **Clone or download**.
		Asegúrese de usar *Clone with HTTPS*.
	-	Abra una sesión de Bash en su ordenador personal y vaya al directorio **PAV**. Desde ahí, ejecute:

		```.sh
		git clone dirección-del-fork-de-la-práctica
		```

	-	Vaya al directorio de la práctica `cd P2`.

	-	Cambie a la rama **fulano-mengano** con la orden:

		```.sh
		git checkout fulano-mengano
		```

*	A partir de este momento, todos los miembros del grupo de prácticas pueden trabajar en su directorio
	local del modo habitual, usando el repositorio remoto en GitHub como repositorio central para el trabajo colaborativo
	de los distintos miembros del grupo de prácticas o como copia de seguridad.
	-	Puede *confirmar* versiones del proyecto en su directorio local con las órdenes siguientes:

		```.sh
		git add .
		git commit -m "Mensaje del commit"
		```

	-	Las versiones confirmadas, y sólo ellas, se almacenan en el repositorio y pueden ser accedidas en cualquier momento.

*	Para interactuar con el contenido remoto en GitHub es necesario que los cambios en el directorio local estén confirmados.

	-	Puede comprobar si el directorio está *limpio* (es decir, si la versión actual está confirmada) usando el comando
		`git status`.

	-	La versión actual del directorio local se sube al repositorio remoto con la orden:

		```.sh
		git push
		```

		*	Si el repositorio remoto contiene cambios no presentes en el directorio local, `git` puede negarse
			a subir el nuevo contenido.

			-	En ese caso, lo primero que deberemos hacer es incorporar los cambios presentes en el repositorio
				GitHub con la orden `git pull`.

			-	Es posible que, al hacer el `git pull` aparezcan *conflictos*; es decir, ficheros que se han modificado
				tanto en el directorio local como en el repositorio GitHub y que `git` no sabe cómo combinar.

			-	Los conflictos aparecen marcados con cadenas del estilo `>>>>`, `<<<<` y `====`. Los ficheros correspondientes
				deben ser editados para decidir qué versión preferimos conservar. Un editor avanzado, del estilo de Microsoft
				Visual Studio Code, puede resultar muy útil para localizar los conflictos y resolverlos.

			-	Tras resolver los conflictos, se ha de confirmar los cambios con `git commit` y ya estaremos en condiciones
				de subir la nueva versión a GitHub con el comando `git push`.


	-	Para bajar al directorio local el contenido del repositorio GitHub hay que ejecutar la orden:

		```.sh
		git pull
		```
	
		*	Si el repositorio local contiene cambios no presentes en el directorio remoto, `git` puede negarse a bajar
			el contenido de este último.

			-	La resolución de los posibles conflictos se realiza como se explica más arriba para
				la subida del contenido local con el comando `git push`.



*	Al final de la práctica, la rama **fulano-mengano** del repositorio GitHub servirá para remitir la
	práctica para su evaluación utilizando el mecanismo *pull request*.
	-	Vaya a la página principal de la copia del repositorio y asegúrese de estar en la rama
		**fulano-mengano**.
	-	Pulse en el botón **New pull request**, y siga las instrucciones de GitHub.


Entrega de la práctica.
-----------------------

Responda, en este mismo documento (README.md), los ejercicios indicados a continuación. Este documento es
un fichero de texto escrito con un formato denominado _**markdown**_. La principal característica de este
formato es que, manteniendo la legibilidad cuando se visualiza con herramientas en modo texto (`more`,
`less`, editores varios, ...), permite amplias posibilidades de visualización con formato en una amplia
gama de aplicaciones; muy notablemente, **GitHub**, **Doxygen** y **Facebook** (ciertamente, :eyes:).

En GitHub. cuando existe un fichero denominado README.md en el directorio raíz de un repositorio, se
interpreta y muestra al entrar en el repositorio.

Debe redactar las respuestas a los ejercicios usando Markdown. Puede encontrar información acerca de su
sintáxis en la página web [Sintaxis de Markdown](https://daringfireball.net/projects/markdown/syntax).
También puede consultar el documento adjunto [MARKDOWN.md](MARKDOWN.md), en el que se enumeran los
elementos más relevantes para completar la redacción de esta práctica.

Recuerde realizar el *pull request* una vez completada la práctica.

Ejercicios
----------

### Etiquetado manual de los segmentos de voz y silencio

- Etiquete manualmente los segmentos de voz y silencio del fichero grabado al efecto. Inserte, a 
  continuación, una captura de `wavesurfer` en la que se vea con claridad la señal temporal, el contorno de
  potencia y la tasa de cruces por cero, junto con el etiquetado manual de los segmentos.

	![Senyal marcat](/img/SenyalMarcat.png)

	Podem veure com al primer panell per sota del senyal en forma d'ona tenim les etiquetes que indiquen veu ('V') o silenci ('S'). Seguidament, observem la potència i la ZCR.


- A la vista de la gráfica, indique qué valores considera adecuados para las magnitudes siguientes:

	* Incremento del nivel potencia en dB, respecto al nivel correspondiente al silencio inicial, para
	  estar seguros de que un segmento de señal se corresponde con voz.
		
  		Es pot observar com en els diferents canvis entre silenci i veu (o viceversa) passem de tenir uns -55 dB a uns -7 dB, aproximadament. Per tant, si ens fixem amb els nivells inicials i comprovem si la relació es compleix al llarg de la resta de senyal (com és el cas), l'increment en qüestió hauria de ser d'entre uns 40 i 45 dB. Així podríem estar segurs que estem tractant amb un segment de veu. 

	* Duración mínima razonable de los segmentos de voz y silencio.

		La durada mínima raonable dels segments hauria de ser l'interval temporal més petit entre dues paraules diferents. Podem veure com pel nostre senyal aquest interval és d'al voltant de 200 ms.

	* ¿Es capaz de sacar alguna conclusión a partir de la evolución de la tasa de cruces por cero?

		Des del nostre punt de vista, la ZCR ens permet indentificar en quin moment tenim al·lòfons sords. Nosaltres treballàvem amb la frase següent: "Som el Gerard Ferret i el Guillem Moreno. Avui fa un dia molt maco. Tot i això, potser després plou". Efectivament, veiem com tenim pics per la 's' de "som", la 'f' de "fa", les 't' de "tot" (encara que la segona estigui una mica més sonoritzada), la 'ix' d'"això", la 'p' i la 's' de "potser", la 'p' i la 's' de "després" i la 'p' de "plou".

### Desarrollo del detector de actividad vocal

  - Complete el código de los ficheros de la práctica para implementar un detector de actividad vocal en
  tiempo real tan exacto como sea posible. Tome como objetivo la maximización de la puntuación-F `TOTAL`.

  A continuació, es mostren els fitxers principals modificats per aconseguir la maximització de la puntuació F-Total. La resta estan disponibles consultant el directori pertinent. Creiem que el codi és autocontingut i que els aclariments més importants estan fets en forma de comentari, però tot i així al darrer apartat s'explica amb més detall el funcionament del fitxer "vad.c".

i) Fitxer "vad.h":

```c
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
```

ii) Fitxer "vad.c":

```c
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

```	


iii) Fitxer "pav_analysis.h" (nova creació):

 ```c
#ifndef PAV_ANALYSIS_H
#define PAV_ANALYSIS_H
#define M_PI 3.14159265358979323846

void compute_hamming_window(float *w, int N);
float compute_power(const float *x, unsigned int N);
float compute_am(const float *x, unsigned int N);
float compute_zcr(const float *x, unsigned int N, float fm);

float compute_power_hamming(const float *x, unsigned int N, float fm);
#endif	/* PAV_ANALYSIS_H	*/
```

iv) Fitxer "pav_analysis.c" (nova creació):

```c
#include <math.h>
#include "pav_analysis.h"


float compute_power(const float *x, unsigned int N) {
    
    float sum = 0.0;
    for (unsigned int n = 0; n < N; n++) {
        sum += x[n] * x[n];
    }
    return 10.0 * log10f(sum / N); //AIXO ESTA EN DBs PER AIXO ES NEGATIU
}

float compute_am(const float *x, unsigned int N) {
    float sum = 0.0;
    for (unsigned int n = 0; n < N; n++) {
        sum += fabsf(x[n]);
    }
    return sum / N;
}   

float compute_zcr(const float *x, unsigned int N, float fm) {
    int crossings = 0;
    for (unsigned int n = 1; n < N; n++) {
        if ((x[n] >= 0 && x[n-1] < 0) || (x[n] < 0 && x[n-1] >= 0)) {
            crossings++;
        }
    }
    return (fm / 2.0f) * ((float)crossings / (N - 1));
}
float compute_power_hamming(const float *x, unsigned int N, float fm) {
    float sum = 0.0f;
    float sum_w2 = 0.0f;

    // Aplicar la ventana de Hamming al único bloque de longitud N
    for (unsigned int n = 0; n < N; n++) {
        float w = 0.54f - 0.46f * cosf((2.0f * 3.14159 * n) / (N - 1));
        sum += (x[n] * w) * (x[n] * w);
        sum_w2 += w * w;
    }
    
    if (sum_w2 == 0.0f) {
        return 2; // or another appropriate value
    }
    return 10.0f * log10f(sum / sum_w2);
}
```

v) Fitxer "main_vad.c":

```c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sndfile.h>
#include "vad.h"
#include "vad_docopt.h"

#define DEBUG_VAD 0x1

int main(int argc, char *argv[]) {
  int verbose = 0;
  SNDFILE *sndfile_in, *sndfile_out = 0;
  SF_INFO sf_info;
  FILE *vadfile;
  int n_read = 0, i;
  VAD_DATA *vad_data;
  VAD_STATE state, last_state;
  float *buffer, *buffer_zeros;
  int frame_size;
  float frame_duration;
  unsigned int t, last_t;
  char *input_wav, *output_vad, *output_wav;
  float alpha0;

  /* Parsear argumentos con docopt */
  DocoptArgs args = docopt(argc, argv, /* help */ 1, /* version */ "2.0");

  verbose    = args.verbose ? DEBUG_VAD : 0;
  input_wav  = args.input_wav;
  output_vad = args.output_vad;
  output_wav = args.output_wav;
  /* Si no se pasa --alpha0, se utiliza el valor por defecto 1.0 */
  alpha0 = (args.alpha0 != NULL ? atof(args.alpha0) : 1.0);

  if (input_wav == NULL || output_vad == NULL) {
    fprintf(stderr, "%s\n", args.usage_pattern);
    return -1;
  }

  /* Abrir fichero de audio de entrada */
  if ((sndfile_in = sf_open(input_wav, SFM_READ, &sf_info)) == NULL) {
    fprintf(stderr, "Error opening input file %s (%s)\n", input_wav, strerror(errno));
    return -1;
  }
  if (sf_info.channels != 1) {
    fprintf(stderr, "Error: the input file must be mono: %s\n", input_wav);
    return -2;
  }

  /* Abrir fichero de salida VAD (.vad) */
  if ((vadfile = fopen(output_vad, "wt")) == NULL) {
    fprintf(stderr, "Error opening output vad file %s (%s)\n", output_vad, strerror(errno));
    return -1;
  }

  /* Abrir fichero de salida de audio si se especifica */
  if (output_wav != NULL) {
    if ((sndfile_out = sf_open(output_wav, SFM_WRITE, &sf_info)) == NULL) {
      fprintf(stderr, "Error opening output wav file %s (%s)\n", output_wav, strerror(errno));
      return -1;
    }
  }

  /* Inicializar VAD para este fichero; cada ejecución es independiente */
  vad_data = vad_open(sf_info.samplerate);
  /* Guardar alpha0 en la estructura (opcional) */
  vad_data->p0 = alpha0;

  /* Asignar memoria para buffers */
  frame_size   = vad_frame_size(vad_data);
  buffer       = (float *) malloc(frame_size * sizeof(float));
  buffer_zeros = (float *) malloc(frame_size * sizeof(float));
  for (i = 0; i < frame_size; ++i)
    buffer_zeros[i] = 0.0F;

  frame_duration = (float) frame_size / (float) sf_info.samplerate;
  last_state = ST_UNDEF;

  for (t = last_t = 0; ; t++) {
    if ((n_read = sf_read_float(sndfile_in, buffer, frame_size)) != frame_size)
      break;

    if (sndfile_out != NULL) {
      /* TODO: copiar samples o escribir ceros en segmentos de silencio */
    }

    state = vad(vad_data, buffer, alpha0);
    if (verbose & DEBUG_VAD)
      vad_show_state(vad_data, stdout);


  // SOLO UN bloque if aquí
  if (state != last_state) {
      if (t != last_t)
          fprintf(vadfile, "%.5f\t%.5f\t%s\n", last_t * frame_duration, t * frame_duration, state2str(last_state));
      last_state = state;
      last_t = t;
  }

    if (sndfile_out != NULL) {
      /* TODO: escribir en el output wav según el estado */
    }
  }

  state = vad_close(vad_data);
  if (t != last_t)
    fprintf(vadfile, "%.5f\t%.5f\t%s\n", last_t * frame_duration, t * frame_duration + n_read / (float) sf_info.samplerate, state2str(state));

  free(buffer);
  free(buffer_zeros);
  sf_close(sndfile_in);
  fclose(vadfile);
  if (sndfile_out != NULL)
    sf_close(sndfile_out);
  return 0;
}
```

- Inserte una gráfica en la que se vea con claridad la señal temporal, el etiquetado manual y la detección
  automática conseguida para el fichero grabado al efecto. 

- Explique, si existen, las discrepancias entre el etiquetado manual y la detección automática.

- Evalúe los resultados sobre la base de datos `db.v4` con el script `vad_evaluation.pl` e inserte a 
  continuación las tasas de sensibilidad (*recall*) y precisión para el conjunto de la base de datos (sólo
  el resumen).

  ![Resum F-score](/img/Resum.png)


### Trabajos de ampliación

#### Cancelación del ruido en los segmentos de silencio

- Si ha desarrollado el algoritmo para la cancelación de los segmentos de silencio, inserte una gráfica en
  la que se vea con claridad la señal antes y después de la cancelación (puede que `wavesurfer` no sea la
  mejor opción para esto, ya que no es capaz de visualizar varias señales al mismo tiempo).

  No realitzat.

#### Gestión de las opciones del programa usando `docopt_c`

- Si ha usado `docopt_c` para realizar la gestión de las opciones y argumentos del programa `vad`, inserte
  una captura de pantalla en la que se vea el mensaje de ayuda del programa.


### Contribuciones adicionales y/o comentarios acerca de la práctica

- Indique a continuación si ha realizado algún tipo de aportación suplementaria (algoritmos de detección o 
  parámetros alternativos, etc.).

  Com que el codi funcionava d'acord amb el que s'havia plantejat, no ens ha calgut afegir aportacions suplementàries.

- Si lo desea, puede realizar también algún comentario acerca de la realización de la práctica que
  considere de interés de cara a su evaluación.

  No tenim cap més comentari a afegir. Restem disponibles per a qualsevol aclariment.


### Antes de entregar la práctica

Recuerde comprobar que el repositorio cuenta con los códigos correctos y en condiciones de ser 
correctamente compilados con la orden `meson bin; ninja -C bin`. El programa generado (`bin/vad`) será
el usado, sin más opciones, para realizar la evaluación *ciega* del sistema.
