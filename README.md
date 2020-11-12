PAV - P3: detección de pitch
============================

Esta práctica se distribuye a través del repositorio GitHub [Práctica 3](https://github.com/albino-pav/P3).
Siga las instrucciones de la [Práctica 2](https://github.com/albino-pav/P2) para realizar un `fork` de la
misma y distribuir copias locales (*clones*) del mismo a los distintos integrantes del grupo de prácticas.

Recuerde realizar el *pull request* al repositorio original una vez completada la práctica.

Ejercicios básicos
------------------

- Complete el código de los ficheros necesarios para realizar la detección de pitch usando el programa
  `get_pitch`.

   * Complete el cálculo de la autocorrelación e inserte a continuación el código correspondiente.

- **Autocorrelación**
    ```
     void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const {

    for (unsigned int l = 0; l < r.size(); ++l) {
  		/// \DONE Compute the autocorrelation r[l]
      for(unsigned int n = 0; n < x.size()-1-l; ++n){
        r[l] = x[n]*x[n+l] + r[l];
      }
        r[l] = (1.0F/x.size())*r[l];
    }

    if (r[0] == 0.0F) //to avoid log() and divide zero 
      r[0] = 1e-10; 
    }
  
  
 - **Enventanado**
      ```
        void PitchAnalyzer::set_window(Window win_type) {
        if (frameLen == 0)
          return;
    
        window.resize(frameLen);
    
        switch (win_type) {
        case HAMMING:{
          /// \DONE Implement the Hamming window
          float a0 = 0.53836F;
          float a1 = 0.46164F;
          for(unsigned int i = 0; i < frameLen; ++i){
            window[i] = a0-a1*cos((2*M_PI*i)/(frameLen-1));
          }
        }  
        break;
    
    
        case RECT:
        default:
          window.assign(frameLen, 1);
        }
      }
        
    
  
 * Inserte una gŕafica donde, en un *subplot*, se vea con claridad la señal temporal de un segmento de
         unos 30 ms de un fonema sonoro y su periodo de pitch; y, en otro *subplot*, se vea con claridad la
    	 autocorrelación de la señal y la posición del primer máximo secundario.
    
    	 NOTA: es más que probable que tenga que usar Python, Octave/MATLAB u otro programa semejante para
    	 hacerlo. Se valorará la utilización de la librería matplotlib de Python.
    
 * Determine el mejor candidato para el periodo de pitch localizando el primer máximo secundario de la
         autocorrelación. Inserte a continuación el código correspondiente.
     - **Seleccionar Periodo de Pitch**
   ```
    void PitchAnalyzer::set_f0_range(float min_F0, float max_F0) {
        npitch_min = (unsigned int) samplingFreq/max_F0;
        if (npitch_min < 2)
          npitch_min = 2;  // samplingFreq/2
    
        npitch_max = 1 + (unsigned int) samplingFreq/min_F0;
    
        //frameLen should include at least 2*T0
        if (npitch_max > frameLen/2)
          npitch_max = frameLen/2;
      }
    
* Implemente la regla de decisión sonoro o sordo e inserte el código correspondiente.
    
     - **Regla Decisión Sonoro/Sordo**
       
    ```
     
      bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm) const {
       
        if ((pot < -39.0F || r1norm < 0.895F) && rmaxnorm < 0.48F)
            return true;
        else
            return false;
      }
    
    
      float PitchAnalyzer::compute_pitch(vector<float> & x) const {
        if (x.size() != frameLen)
          return -1.0F;
    
        //Window input frame
        for (unsigned int i=0; i<x.size(); ++i)
          x[i] *= window[i];
    
        vector<float> r(npitch_max);
    
        //Compute correlation
        autocorrelation(x, r);
    
        vector<float>::const_iterator iR = r.begin(), iRMax = iR;
    
        while(*iR > 0){
          ++iR;
        }
        if (iR<r.begin() + npitch_min)
          iR += npitch_min;
        iRMax = iR;
          while(iR != r.end()){
        if(*iR > *iRMax){
          iRMax = iR; //iRmax: Posición donde está el máximo
        }
        ++iR;
        }
       
    }
    
       
- Una vez completados los puntos anteriores, dispondrá de una primera versión del detector de pitch. El 
  resto del trabajo consiste, básicamente, en obtener las mejores prestaciones posibles con él.

  * Utilice el programa `wavesurfer` para analizar las condiciones apropiadas para determinar si un
    segmento es sonoro o sordo. 
	
	  - Inserte una gráfica con la detección de pitch incorporada a `wavesurfer` y, junto a ella, los 
	    principales candidatos para determinar la sonoridad de la voz: el nivel de potencia de la señal
		(r[0]), la autocorrelación normalizada de uno (r1norm = r[1] / r[0]) y el valor de la
		autocorrelación en su máximo secundario (rmaxnorm = r[lag] / r[0]).

		Puede considerar, también, la conveniencia de usar la tasa de cruces por cero.

	    Recuerde configurar los paneles de datos para que el desplazamiento de ventana sea el adecuado, que
		en esta práctica es de 15 ms.

      - Use el detector de pitch implementado en el programa `wavesurfer` en una señal de prueba y compare
	    su resultado con el obtenido por la mejor versión de su propio sistema.  Inserte una gráfica
		ilustrativa del resultado de ambos detectores.
		
		  * El mejor caso corresponde a la señál femenina sb022.wav con un 95.02 %
		    <img src="BEST_CASE.PNG">
		    
		  * Podemos comparar el detector con el detector del Wavesurfer
		<img src="best_case_wavesurfer.PNG">
		
  
  * Optimice los parámetros de su sistema de detección de pitch e inserte una tabla con las tasas de error y el *score* TOTAL proporcionados por `pitch_evaluate` en la evaluación de la base de datos 
	`pitch_db/train`.. 
	
   
 **Tabla Tasa de error y *score* TOTAL con optimización**
 
 - Ambos

Tipo de error | Tasa de error 
------------- | -------------
Unvoiced frames as voiced | 5.56 %
Voiced frames as unvoiced | 9.07 %
Gross voiced errors (+20%) | 2.04 %
MSE of fine errors | 2.32 %

- Hombres



Tipo de error | Tasa de error
------------- | -------------
Unvoiced frames as voiced | 6.35 %
Voiced frames as unvoiced | 14.18 %
Gross voiced errors (+20%) | 3.09 %
MSE of fine errors | 2.16 %

- Mujeres

Tipo de error | Tasa de error
------------- | -------------
Unvoiced frames as voiced | 4.94 %
Voiced frames as unvoiced | 4.51 %
Gross voiced errors (+20%) | 1.19 %
MSE of fine errors | 2.48 %


**Sexo** | **_score_ TOTAL** 
---------|------------------
 Ambos | **90.10 %**
 Mujeres | **87.49 %**
 Hombres | **92.25 %**


***
   * Inserte una gráfica en la que se vea con claridad el resultado de su detector de pitch junto al del
     detector de Wavesurfer. Aunque puede usarse Wavesurfer para obtener la representación, se valorará
	 el uso de alternativas de mayor calidad (particularmente Python).

------------------------
------------------------

Ejercicios de ampliación
------------------------

- Usando la librería `docopt_cpp`, modifique el fichero `get_pitch.cpp` para incorporar los parámetros del
  detector a los argumentos de la línea de comandos.
  
  Esta técnica le resultará especialmente útil para optimizar los parámetros del detector. Recuerde que
  una parte importante de la evaluación recaerá en el resultado obtenido en la detección de pitch en la
  base de datos.

  * Inserte un *pantallazo* en el que se vea el mensaje de ayuda del programa y un ejemplo de utilización
    con los argumentos añadidos.

- Implemente las técnicas que considere oportunas para optimizar las prestaciones del sistema de detección
  de pitch.

  Entre las posibles mejoras, puede escoger una o más de las siguientes:

  * Técnicas de preprocesado: filtrado paso bajo, *center clipping*, etc.
  * Técnicas de postprocesado: filtro de mediana, *dynamic time warping*, etc.
  * Métodos alternativos a la autocorrelación: procesado cepstral, *average magnitude difference function*
    (AMDF), etc.
  * Optimización **demostrable** de los parámetros que gobiernan el detector, en concreto, de los que
    gobiernan la decisión sonoro/sordo.
  * Cualquier otra técnica que se le pueda ocurrir o encuentre en la literatura.

  Encontrará más información acerca de estas técnicas en las [Transparencias del Curso](https://atenea.upc.edu/pluginfile.php/2908770/mod_resource/content/3/2b_PS%20Techniques.pdf)
  y en [Spoken Language Processing](https://discovery.upc.edu/iii/encore/record/C__Rb1233593?lang=cat).
  También encontrará más información en los anexos del enunciado de esta práctica.

  Incluya, a continuación, una explicación de las técnicas incorporadas al detector. Se valorará la
  inclusión de gráficas, tablas, código o cualquier otra cosa que ayude a comprender el trabajo realizado.

  También se valorará la realización de un estudio de los parámetros involucrados. Por ejemplo, si se opta
  por implementar el filtro de mediana, se valorará el análisis de los resultados obtenidos en función de
  la longitud del filtro.
   

Evaluación *ciega* del detector
-------------------------------

Antes de realizar el *pull request* debe asegurarse de que su repositorio contiene los ficheros necesarios
para compilar los programas correctamente ejecutando `make release`.

Con los ejecutables construidos de esta manera, los profesores de la asignatura procederán a evaluar el
detector con la parte de test de la base de datos (desconocida para los alumnos). Una parte importante de
la nota de la práctica recaerá en el resultado de esta evaluación.
