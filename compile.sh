docker start TPE
docker exec -ti TPE make clean -C/root/Toolchain
docker exec -ti TPE make clean -C/root/
docker exec -ti TPE make -C/root/Toolchain
docker exec -ti TPE make -C/root/
docker stop TPE