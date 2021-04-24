# Finding Treasure booteable

_Juego de aliens que siguen un laberinto en x86_

## Comenzando 🚀

Mira **Deployment** para conocer como desplegar el proyecto.


### Pre-requisitos 📋

_Se necesita el emulador Qemu_

```
sudo apt install qemu qemu-utils qemu-kvm virt-manager
libvirt-daemon-system libvirt-clients bridge-utils
sudo apt install gnome-system-tools
```

_Ademas el compilador NASM_
```
sudo apt-get install nasm
```

## Despliegue 📦

_Se compila y ejecuta mediante el comando_

```
make
```

El booteable se realiza mediante el quemado del binario de la siguiente manera:
```
sudo dd if=bootable_tanks.bin of=/dev/sdb bs=512 count=3
```



## Autores ✒️

* **Emanuel Esquiel** - [ema11412](https://github.com/ema11412)
* **Roger Valderrama** - [germago119](https://github.com/germago119)
* **Gabriel Alfaro** - [gabofaro](https://github.com/gabofaro9)