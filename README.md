# libre-blog

**Libre-blog** is a blog system. This repository uses the [Drogon](https://github.com/drogonframework/drogon) framework to implement the **backend API** of libre-blog.

## API List
- article
    - Get `/articles`
    - Get `/admin/articles`
    - Get `/articles/category/{slug}`
    - Get `/articles/tag/{slug}`
    - Get `/articles/user/{id}`
    - Get `/article/{id}`
    - Post `/admin/article`
    - Put `/admin/article`
    - Delete `/admin/article/`
- category
    - Get `/categories`
    - Get `/category/{id}`
    - Post `/admin/category`
    - Put `/admin/category`
    - Delete `/admin/category`
- tag
    - Get `/tags`
    - Get `/tag/{id}`
    - Post `/admin/tag`
    - Put `/admin/tag`
    - Delete `/admin/tag`
- user
    - Post `/login`
    - Post `/register`
    - Get `/admin/user/center`
    - Patch `/admin/user/general-info`
    - Patch `/admin/user/password`
    - Get `/admin/users`
    - Get `/user/{id}`
    - Post `/admin/user`
    - Put `/admin/user`
    - Delete `/admin/user`
    - Get `/admin/user/role`
- role
    - Get `/roles`
- menu
    - Get `/admin/menu`
    - Get `/menu`

**For more detailed API documentation, please see [👉here](./docs/libre-blog_api.md).**

## Setup, Build, and Run
### Setup Environment
This project is developed on *Debian GNU/Linux 12 (bookworm)*. You need to install these packages to meet the development environment.
```shell
apt install g++ gdb libgtest-dev git cmake doxygen \
    libjsoncpp-dev uuid-dev zlib1g-dev \
    libssl-dev openssl libmariadb-dev libhiredis-dev libc-ares-dev libbrotli-dev libyaml-cpp-dev \
    -y
```

Then, you need to install the Drogon framework. For more detailed information about the Drogon framework, please see the [Drogon wiki page](https://github.com/drogonframework/drogon/wiki/).
```shell
git clone https://github.com/drogonframework/drogon
cd drogon
git submodule update --init
mkdir build
cd build
cmake ..
make && sudo make install
```

This project uses [`jwt-cpp`](https://github.com/Thalhammer/jwt-cpp) for token issuing and verification. So, you need to install this library.
```shell
git clone https://github.com/Thalhammer/jwt-cpp.git
cd jwt-cpp
mkdir build
cd build
cmake .. && make
sudo make install
```

### Build
#### 0.Clone this repository
```bash
git clone https://github.com/fansuregrin/libre-blog.git
cd libre-blog
```

#### 1.Create database and tables

Please see [🛠️here](./sql/README.md)

#### 2.Build Executable File

```shell
mkdir build
cd build
cmake .. && make
```

### Run
Just run the `libre-blog` executable file in the `build` folder.
```shell
cd build
./libre-blog
```

## Acknowledgements
- Drogon: Provides the possibility to build various types of web application server programs.
- VSCode: Provide an environment for code writing and debugging.
- ApiPost: Provide API design and debugging functions.