# Database configuration

Enter the `sql/` directory under the project:
```bash
cd ./sql
```

```sql
CREATE DATABASE libre_blog;
USE libre_blog;
source ./create_tables.sql;
source ./gen_menus.sql;
source ./gen_roles.sql;
-- add default category
INSERT INTO `category` (`id`,`slug`,`name`) VALUE (1,'uncategoried','未分类');
-- add administrator
SET @password = '22FE07BD414147E4EFDF7117803F77C22EAEB4C66932357161F3F6A11978FF900E77394236EF1C8694864769D76A3870';
INSERT INTO `user` (`id`,`username`,`password`,`email`,`realname`,`role`) VALUE
    (1, 'admin', @password, 'admin@example.com', 'Bob Wood', 1);
```