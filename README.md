# 💻 Operating Systems Project, 3rd year, CTI-RO UPT

## 📖 Project description
 This project is a simple C program designed to perform a non-recursive scan of a specified directory, providing statistics on the types of files found. The program utilizes parallel processing to create separate processes for different tasks, such as generating statistics, transforming BMP files to grayscale, and counting correct sentences in regular files.

.

## 🏃‍♀️ How to run?
#### 1. Clone repository 
```shell
    git clone https://github.com/zsoltzsolt/Proiect_SO.git
```
#### 2. Get inside Project_SO directory
```shell
    cd Proiect_SO
```

#### 3. Run shell script
```shell
    ./scripts/run.sh <input_directory_path> <output_directory_path> <alphanumeric_character>
```

## 🧐 Example output

### 1. Regular file statistics
```text
    nume fisier: fisier.txt
    dimensiune: 18 octeti
    identificatorul utilizatorului: 501
    timpul ultimei modificari: 09.12.2023
    contorul de legaturi: 1
    drepturi de accces user: RW-
    drepturi de accces grup: R--
    drepturi de accces altii: R--

```

### 2. Symbolic link statistics
```text
    nume legatura: link2
    dimensiune legatura: 10 octeti
    dimensiune fisier: 18 octeti
    drepturi de accces user: RWX
    drepturi de accces grup: R-X
    drepturi de accces altii: R-X
```

### 3. BMP image statistics
```
    nume fisier: imagine_2.bmp
    inaltime: 426
    latime: 640
    dimensiune: 818058 octeti
    identificatorul utilizatorului: 501
    timpul ultimei modificari: 09.12.2023
    contorul de legaturi: 1
    drepturi de accces user: RW-
    drepturi de accces grup: R--
    drepturi de accces altii: R--
```

### 4. Directory statisctics
```text
    nume director: dir
    identificatorul utilizatorului: 501
    drepturi de access user: RWX
    drepturi de access grup: R-X
    drepturi de access altii: R-X
```

### 5. Original vs Grayscale image
![alt text](imagine.bmp)
![alt text](input/imagine.bmp)


