
sudo chmod -R 777 ./*
sudo docker start tp2                                           #start del docker

sudo docker exec -it tp2   make -C/root/Toolchain                       #make
sudo docker exec -it tp2   make -C/root/                                #make

sudo chmod 777 Image/x64BareBonesImage.qcow2                       #permisos
sudo chmod -R 777 ./*

sudo docker exec -it tp2   make clean -C/root/Toolchain                 #borra lo que habia
sudo docker exec -it tp2   make clean -C/root/                          #borra lo que habia
sudo docker stop tp2