# Servicios en MV

_Ecualizador de imagenes, como servicio del sistema operativo_

## Comenzando 🚀

Mira **Deployment** para conocer como desplegar el proyecto.


### Pre-requisitos 📋

_Se necesita la bilbioteca LibMagicWand_

```
sudo apt-get install libmagickwand-dev
```

## Despliegue 📦

_Se compila mediante el comando_

```
make
```

Para el despliegue del Daemon, dentro de la carpeta ``` /bin```

```
sudo cp ImageServer.service /lib/systemd/system/
sudo cp ImageServer-* /usr/bin
sudo chmod +x /usr/bin/ImageServer-*
```

Iniciar y detener

```
sudo systemctl start ImageServer
sudo systemctl status ImageServer
```

Habilitar arranque con el sistema
```
sudo systemctl enable ImageServer 
```

## UsoWebApp 🚀

_Se ejecuta ```/Web/view/index.html```_

Presionar _enviar_, luego _procesar_.


## Autores ✒️

* **Emanuel Esquiel Lopez** - [ema11412](https://github.com/ema11412)