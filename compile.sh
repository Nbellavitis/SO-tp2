docker start tp2
docker exec -ti tp2 make clean -C/root/Toolchain
docker exec -ti tp2 make clean -C/root/
docker exec -ti tp2 make -C/root/Toolchain
docker exec -ti tp2 make -C/root/
