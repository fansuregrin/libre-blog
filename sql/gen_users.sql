SET @uuid = UUID();
INSERT INTO `user` (`username`,`password`,`email`,`realname`,`salt`,`role`) VALUES
    ('bobwood', UPPER(SHA2(CONCAT('bw1234', @uuid),256)), 'bobwood@ouc.edu.cn', 'Bob Wood', @uuid, 1);
SET @uuid = UUID();
INSERT INTO `user` (`username`,`password`,`email`,`realname`,`salt`) VALUES
    ('yuhua', UPPER(SHA2(CONCAT('yuhua1234', @uuid),256)), 'yuhua@bnu.edu.cn', '小华', @uuid, 2);
INSERT INTO `user` (`username`,`password`,`email`,`realname`,`salt`) VALUES
    ('moyan', UPPER(SHA2(CONCAT('moyan1234', @uuid),256)), 'moyan@bnu.edu.cn', '小莫', @uuid, 2);
INSERT INTO `user` (`username`,`password`,`email`,`realname`,`salt`) VALUES
    ('tom1997', UPPER(SHA2(CONCAT('ts1234', @uuid),256)), 'ts@gmail.com', 'Tom Smith', @uuid, 3);
INSERT INTO `user` (`username`,`password`,`email`,`realname`,`salt`) VALUES
    ('dijia', UPPER(SHA2(CONCAT('dj6666', @uuid),256)), 'dijia@m78.galaxy', '迪迪', @uuid, 4);