#!/bin/bash

for i in {1..50}
do
  echo "Ejecutando iteración $i"
  timeout 20s ./externAppExecutable

  if [ $? -eq 124 ]; then
    echo "El programa fue terminado porque excedió los 20 segundos"
  fi
done