-- 默认密码：Pwz123#
SET @password = '22FE07BD414147E4EFDF7117803F77C22EAEB4C66932357161F3F6A11978FF900E77394236EF1C8694864769D76A3870';
SET @administrator = 1;
SET @editor = 2;
SET @contributor = 3;
SET @subscriber = 4;
INSERT INTO `user` (`username`,`password`,`email`,`realname`,`salt`,`role`) 
VALUES
    ('bobwood', @password, 'bobwood@ouc.edu.cn', 'Bob Wood', @administrator),
    ('xiaohua', @password, 'xiaohua@bnu.edu.cn', '小华', @editor),
    ('xiaomo', @password, 'xiaoyan@bnu.edu.cn', '小莫', @editor),
    ('tom1997', @password, 'ts@gmail.com', 'Tom Smith', @contributor),
    ('dijia', @password, 'dijia@m78.galaxy', '迪迪', @subscriber);