# libre-blog

**Libre-blog** is a blog system. This repository uses the [Drogon](https://github.com/drogonframework/drogon) framework to implement the **backend API** of libre-blog. The front-end implementation of libre-blog is [👉here](https://github.com/fansuregrin/libre-blog-frontend).

## API List
- `/blog/page/{page}`
- `/blog/category/{slug}/{page}`
- `/blog/user/{id}/{page}`
- `/blog/tag/{slug}/{page}`
- `/blog/admin/page/{page}`
- `/blog/article/{id}`
- `/blog/article/update`
- `/blog/article/delete`
- `/blog/category`
- `/login`
- `/user/center`
- `/user/add`
- `/user/update`
- `/user/update/password`
- `/user/role`

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

### Build
1. Create database and data tables
Please see [🛠️here](./sql/create_tables.sql)
2. Generate database model code
```shell
git clone https://github.com/fansuregrin/libre-blog.git
cd libre-blog/models
dg_ctl create model .
```
3. Build Executable File
```shell
cd libre-blog/
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