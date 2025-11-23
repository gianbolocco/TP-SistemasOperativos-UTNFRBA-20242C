cd ~

git clone https://github.com/sisoputnfrba/so-commons-library
cd ./so-commons-library
make debug
make install

cd ~
mkdir /home/utnso/scripts-pruebas
cp -r /home/utnso/tp-2024-2c-B.E.G.G/pruebas/scripts/. /home/utnso/scripts-pruebas

cd ~
mkdir /home/utnso/mount_dir
mkdir /home/utnso/mount_dir/files
