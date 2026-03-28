# Tugas Kecil 2 - Strategi Algoritma IF2211

Program voxelisasi mesh 3D menggunakan algoritma *Divide and Conquer*. Menerima input file `.obj` dan mengkonversi mesh (faces & vertices) menjadi representasi voxel.

## Requirements

- C++17
- CMake 3.10+
- MinGW / g++ (Windows) atau GCC (Linux)

## Build
```bash
mkdir bin && cd bin

# CMAKE - Windows
cmake .. -G "MinGW Makefiles" 
mingw32-make                  

# CMAKE - Linux
cmake ..                      
make                         
```

## Cara Penggunaan

Jalankan executable file dari folder `build`:
```bash
./main.exe   # Windows
./main       # Linux
```

Input : 
1. File .obj ditaruh di folder **test**
2. File .obj yang diperhatikan hanya v (vertex) dan f (faces)
3. Menentukan depth (sebagai tingkat detail dari hasil konversi), bisa juga untuk tidak memilih.

Ikuti langkah berikut:
1. Pilih **Opsi 1** untuk konversi mesh ke voxel, atau **Opsi 2** untuk visualisasi objek
2. Masukkan nama file `.obj` yang ingin dikonversi (Hanya nama file saja tanpa adanya tambahan ekstensi .obj)
3. Tentukan **depth** (tingkat detail voxel) jika ingin mengatur hasil
4. Masukkan nama file output untuk ekspor hasil

[WARNING]
File .obj yang valid adalah : 
1. Faces adalah segitiga
2. Hanya memperhitungkan vertex dan faces

## Author

[Leonardus Brain Fatolosja](https://www.github.com/leains) — 13524146 (Teknik Informatika)