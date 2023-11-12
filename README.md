# 💻 Operating Systems Project, 3rd year, CTI-RO UPT

## 📖 Project description
 It is a straightforward C program designed to perform a non-recursive scan of a specified directory, providing statistics on the types of files found, and saving the statistical information in a file named ```statistici.txt```.

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
    ./scripts/run.sh <directory path>
```

## 🧐 Example output

### 1. Regular file 
```text
    nume fisier: input/fisier.txt
    dimensiune: 12 octeti
    numar legaturi: 501
    identificatorul utilizatorului: 501
    timul ultimei modificari: 12.11.2023
    drepturi de accces user: RW-
    drepturi de accces grup: R--
    drepturi de accces altii: R--
```

### 2. Symbolic link
```text
    nume legatura: input/image_link
    dimensiune legatura: 11
    dimensiune fisier: 7372938
    drepturi de accces user: RWX
    drepturi de accces grup: R-X
    drepturi de accces altii: R-X
```

### 3. BMP image
```
    nume fisier: input/imagine.bmp
    inaltime: 1280
    latime: 1920
    dimensiune: 7372938 octeti
    numar legaturi: 501
    identificatorul utilizatorului: 501
    timul ultimei modificari: 12.11.2023
    drepturi de accces user: RW-
    drepturi de accces grup: R--
    drepturi de accces altii: R--
```

### 4. Directory
```text
    nume director: input/directory
    identificatorul utilizatorului: 501
    drepturi de access user: RWX
    drepturi de access grup: R-X
    drepturi de access altii: R-X
```


