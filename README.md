# Carrito Esquivador de Obstáculos con Arduino

Carrito robótico autónomo que avanza hasta detectar un obstáculo, se detiene, **explora con un sensor ultrasónico montado sobre un servomotor** (izquierda y derecha) y gira hacia el lado más despejado. Si ambos lados están bloqueados, retrocede y da media vuelta.

## Características

- Detección de obstáculos al frente con sensor ultrasónico.
- Sensor montado sobre un servo para "mirar" a la izquierda y a la derecha.
- Decisión automática de dirección comparando distancias laterales.
- Maniobra de escape (retroceso + media vuelta) cuando no hay salida.
- Promedio de 3 lecturas laterales para reducir ruido.
- Parámetros configurables al inicio del código (velocidad, distancia, ángulos y tiempos de giro).
- Salida por monitor serie para depuración.

## Componentes

| Componente | Cantidad | Nota |
|---|---|---|
| Arduino UNO-R3 | 1 | Los pines PWM usados son 5 y 6 |
| Sensor ultrasónico (HC-SR04) | 1 | Medición de distancia |
| Servomotor (SG90 o similar) | 1 | Gira el sensor |
| Puente H (L298N) | 1 | Controla los 2 motores |
| Motores reductores DC con ruedas | 2 | Tracción diferencial |
| Protoborad | 1 | Para conectar los componentes |
| Chasis, rueda loca | 1,3 | Base para montar el protoboard y componentes |
| Batería / portapilas | 1 | Alimentación de motores |
| Cables jumper | - | -|

## Conexiones

| Elemento | Pin Arduino |
|---|---|
| Sensor – Trigger | 12 |
| Sensor – Echo | 8 |
| Servo – Señal | 9 |
| Motor 1 – IN1 | 2 |
| Motor 1 – IN2 | 7 |
| Motor 1 – ENA (PWM) | 6 |
| Motor 2 – IN3 | 4 |
| Motor 2 – IN4 | 3 |
| Motor 2 – ENB (PWM) | 5 |


## Funcionamiento

1. El carro **avanza** mientras la distancia frontal sea mayor o igual a `DIST_OBSTACULO`.
2. Si detecta un obstáculo, **se detiene**.
3. El servo gira el sensor a la **izquierda** y mide; luego a la **derecha** y mide; después regresa al **centro**.
4. Decide:
   - Si la distancia en ambos lados < `DIST_OBSTACULO`, entonces **retrocede** y da **media vuelta**.
   - Si la distancia a la izquierda más libre, entonces **gira a la izquierda**.
   - Si la distancia a la derecha más libre, entonces **gira a la derecha**.
5. Se detiene brevemente y vuelve a empezar.

## Instalación y uso

1. Instala el [Arduino IDE](https://www.arduino.cc/en/software).
2. Arma el circuito según la tabla de conexiones.
3. Abre el archivo `.ino` del proyecto en el IDE (la librería `Servo` viene incluida).
4. Selecciona tu placa y puerto en **Herramientas**.
5. Sube el código.
6. Configurando el **Monitor Serie** a **9600 baudios** se muestran las distancias medidas.

## Parámetros configurables

Se encuentran al inicio del código:

| Constante | Valor por defecto | Descripción |
|---|---|---|
| `VELOCIDAD` | 150 | PWM de los motores (0–255) |
| `DIST_OBSTACULO` | 20.0 | Distancia (cm) a la que se considera obstáculo |
| `ANG_IZQ` | 5 | Ángulo del servo mirando a la izquierda |
| `ANG_DER` | 175 | Ángulo del servo mirando a la derecha |
| `ANG_CENTRO` | 90 | Ángulo del servo mirando al frente |
| `T_GIRO_90` | 600 | Tiempo (ms) para girar ~90° |
| `T_GIRO_180` | 1200 | Tiempo (ms) para media vuelta |

## Estructura del código

| Función | Descripción |
|---|---|
| `avanzar()`, `retroceder()` | Movimiento lineal de frente |
| `giro_izquierda()`, `giro_derecha()` | Giros sobre su propio eje vertical |
| `detener()` | Detiene ambos motores |
| `GirarIzquierdaSensor()`, `GirarDerechaSensor()`, `CentrarSensor()` | Orientan el servomotor |
| `medirDistancia()` | Una lectura del pulso ultrasónico |
| `medirPromedio()` | Promedio de 3 lecturas |
| `loop()` | Lógica de decisión principal |