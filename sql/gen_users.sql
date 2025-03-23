-- 默认密码：Pw123#
SET @password = '169EAD658AA375192BFECCFE28F2F275C59574B3CF3BFCFA6727441FE5E89B30F42B89BC19598910BA2EE135BB3ECA08';
SET @administrator = 1;
SET @editor = 2;
SET @contributor = 3;
SET @subscriber = 4;
INSERT INTO `user` (`username`,`password`,`email`,`realname`,`role`) 
VALUES
    ('bobwood', @password, 'bobwood@ouc.edu.cn', 'Bob Wood', @administrator),
    ('xiaohua', @password, 'xiaohua@bnu.edu.cn', '小华', @editor),
    ('xiaomo', @password, 'xiaoyan@bnu.edu.cn', '小莫', @editor),
    ('tom1997', @password, 'ts@gmail.com', 'Tom Smith', @contributor),
    ('dijia', @password, 'dijia@m78.galaxy', '迪迪', @subscriber);