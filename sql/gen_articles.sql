delimiter //
CREATE PROCEDURE `genArticles`()
BEGIN
    declare i int default 1;
    declare time datetime default '2021-7-1 00:00:00';
    declare author_id int default 1;
    declare category_id int default 1;

    while i <= 100 do
        INSERT INTO article 
        (`title`,`author_id`,`category_id`,`content`,`create_time`, `modify_time`) VALUES 
        (CONCAT('标题',i), FLOOR(RAND()*5)+1, FLOOR(RAND()*6)+1, CONCAT('我是内容',i), time, time);
        set i = i + 1;
        set time = DATE_ADD(time, INTERVAL FLOOR(RAND()*1000000)+1 SECOND);
    end while;
END
//
delimiter ;