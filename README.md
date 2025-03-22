# libre-blog

**Libre-blog** is a blogging platform powered by the [Drogon](https://github.com/drogonframework/drogon) framework for its backend API.

## API Endpoints
- **Article**
    - GET `/articles`
    - GET `/admin/articles`
    - GET `/articles/category/{slug}`
    - GET `/articles/tag/{slug}`
    - GET `/articles/user/{id}`
    - GET `/article/{id}`
    - POST `/admin/article`
    - PUT `/admin/article`
    - DELETE `/admin/article/`
- **Category**
    - GET `/categories`
    - GET `/category/id/{id}`
    - GET `/category/slug/{slug}`
    - POST `/admin/category`
    - PUT `/admin/category`
    - DELETE `/admin/category`
- **Tag**
    - GET `/tags/all`
    - GET `/tags`
    - GET `/tag/id/{id}`
    - GET `/tag/slug/{slug}`
    - POST `/admin/tag`
    - PUT `/admin/tag`
    - DELETE `/admin/tag`
- **User**
    - POST `/login`
    - POST `/register`
    - GET `/admin/user/center`
    - PATCH `/admin/user/general-info`
    - PATCH `/admin/user/password`
    - GET `/admin/users`
    - GET `/user/{id}`
    - POST `/admin/user`
    - PUT `/admin/user`
    - DELETE `/admin/user`
- **Role**
    - GET `/admin/roles`
- **Menu**
    - GET `/admin/menu`

## Setup, Build, and Run
### Setup Environment
This project is developed on *Debian GNU/Linux 12 (bookworm)* or *Arch Linux*. 
Install the following packages to set up the development environment:

On Debian:
```shell
apt install \
    g++ gdb libgtest-dev git cmake doxygen \
    libjsoncpp-dev uuid-dev zlib1g-dev \
    libssl-dev openssl libmariadb-dev libhiredis-dev libc-ares-dev libbrotli-dev libyaml-cpp-dev \
    -y
```

On Arch:
```shell
pacman install \
    g++ gdb gtest git cmake doxygen \
    jsoncpp util-linux-libs zlib \
    openssl mariadb-libs hiredis c-ares brotli libyaml
```

Next, install the Drogon framework. 
For detailed information, refer to the [Drogon wiki page](https://github.com/drogonframework/drogon/wiki/).
```shell
git clone https://github.com/drogonframework/drogon
cd drogon
git submodule update --init
mkdir build
cd build
cmake ..
make && sudo make install
```

This project also uses [`jwt-cpp`](https://github.com/Thalhammer/jwt-cpp) for token issuing and verification. Install this library as follows:
```shell
git clone https://github.com/Thalhammer/jwt-cpp.git
cd jwt-cpp
mkdir build
cd build
cmake .. && make
sudo make install
```

### Build
#### 0. Clone this repository
```bash
git clone https://github.com/fansuregrin/libre-blog.git
cd libre-blog
```

#### 1. Create database and tables
Refer to [🛠️here](./sql/README.md) for instructions.

#### 2. Build the executable file
```shell
mkdir build
cd build
cmake .. && make
```

### Run
Execute the `libre-blog` file located in the `build` folder:
```shell
cd build
./libre-blog
```

## Acknowledgements
- **Drogon**: Enables the creation of various types of web application server programs.
- **VSCode**: Provides an environment for code writing and debugging.
- **ApiPost**: Supports API design and debugging.
- **DataGrip**: Provides a powerful database management tool that simplifies database development and administration.