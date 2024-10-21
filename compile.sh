MM="BUDDY"
docker start tp2
docker exec tp2 /bin/bash -c cd /root/Toolchain && make clean all MM=$MM
docker exec tp2 /bin/bash -c cd /root && make all MM=$MM
docker exec tp2 /bin/bash -c cd /root && chmod 777 Image/x64BareBonesImage.qcow2
