# Demostración de Yolo-v5

## Exportar modelo rknn

Consulte https://github.com/airockchip/rknn_model_zoo/tree/main/models/vision/object_detection/yolov5-pytorch



## Precauciones

1. Utilice la versión rknn-toolkit2 mayor o igual a 1.1.2.
2. Cuando cambie a un modelo entrenado por usted mismo, preste atención a alinear los parámetros de posprocesamiento, como el ancla; de lo contrario, se producirán errores de análisis de posprocesamiento.
3. Tanto el sitio web oficial como los modelos de preentrenamiento de rk tienen el objetivo de detectar 80 categorías. Si entrena el modelo usted mismo, debe cambiar los parámetros de posprocesamiento OBJ_CLASS_NUM y NMS_THRESH, BOX_THRESH en include/postprocess.h.
5. La demostración requiere el soporte de librga.so. Para su compilación y uso, consulte https://github.com/rockchip-linux/linux-rga
5. Debido a limitaciones de hardware, el modelo de demostración traslada la parte de posprocesamiento del modelo yolov5 a la implementación de CPU de forma predeterminada. Todos los modelos incluidos en esta demostración utilizan relu como función de activación. En comparación con la función de activación silu, la precisión es ligeramente menor y el rendimiento ha mejorado significativamente.



## Demostración de Android

### compilar

Modifique la ruta del NDK de Android `ANDROID_NDK_PATH` en `build-android_<TARGET_PLATFORM>.sh` según la plataforma especificada. <TARGET_PLATFORM> puede ser RK356X o RK3588. Por ejemplo, cámbielo a:

```sh
ANDROID_NDK_PATH=~/opt/tool_chain/android-ndk-r17
```

Luego ejecuta:

```sh
./build-android_<TARGET_PLATFORM>.sh
```

### Empuje el archivo ejecutable al tablero

Conecte el puerto USB de la placa a la PC y mueva todo el directorio de demostración a `/data`:

```sh
raíz adb
remontar adb
adb push install/rknn_yolov5_demo /datos/
```

### Correr

```sh
shell adb
cd /datos/rknn_yolov5_demo/

exportar LD_LIBRARY_PATH=./lib
./rknn_yolov5_demo model/<TARGET_PLATFORM>/yolov5s-640-640.rknn model/bus.jpg
```

## Demostración de Aarch64 Linux

### compilar

Modifique la ruta `TOOL_CHAIN` en el directorio donde se encuentra el compilador cruzado en `build-linux_<TARGET_PLATFORM>.sh` según la plataforma especificada, por ejemplo, cámbiela a

```sh
exportar TOOL_CHAIN=~/opt/tool_chain/gcc-9.3.0-x86_64_aarch64-linux-gnu/host
```

Luego ejecuta:

```sh
./build-linux_<TARGET_PLATFORM>.sh
```

### Empuje el archivo ejecutable al tablero


Copie install/rknn_yolov5_demo_Linux al directorio /userdata/ de la placa.

- Si usas la placa EVB de Rockchip, puedes usar adb para enviar archivos a la placa:

```
adb push install/rknn_yolov5_demo_Linux /userdata/
```

- Si usa otras placas, puede usar scp u otros métodos para copiar install/rknn_yolov5_demo_Linux al directorio /userdata/ de la placa

### Correr

```sh
shell adb
cd /datos de usuario/rknn_yolov5_demo_Linux/

exportar LD_LIBRARY_PATH=./lib
./rknn_yolov5_demo model/<TARGET_PLATFORM>/yolov5s-640-640.rknn model/bus.jpg
```

Nota: Intente buscar la ubicación de librga.so y agréguela a LD_LIBRARY_PATH si librga.so no se encuentra en la carpeta lib.
Usando los siguientes comandos para agregar a LD_LIBRARY_PATH.

```sh
exportar LD_LIBRARY_PATH=./lib:<LOCATION_LIBRGA.SO>
```



### Aviso

- Debe seleccionar la biblioteca librga correcta según el controlador rga del sistema. Para dependencias específicas, consulte: https://github.com/airockchip/librga
