# Database configuration

```sql
CREATE DATABASE libre_blog;
source ./create_tables.sql;
source ./gen_menus.sql;
source ./gen_roles.sql;
source ./gen_categories.sql;
-- add the administrator to the user table
SET @uuid = UUID();
INSERT INTO `user` (`username`,`password`,`email`,`realname`,`salt`,`role`) VALUES ('bobwood', UPPER(SHA2(CONCAT('bw1234', @uuid),256)), 'bobwood@ouc.edu.cn', 'Bob Wood', @uuid, 1);
```