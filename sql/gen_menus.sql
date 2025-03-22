-- 每个角色的顶级菜单
INSERT INTO menu(`id`,`label`,`name`,`parent`, `ancestor`) VALUES 
  (1, 'administrator', 'administrator', NULL, NULL),
  (2, 'editor', 'editor', NULL, NULL),
  (3, 'contributor', 'contributor', NULL, NULL),
  (4, 'subscriber', 'subscriber', NULL, NULL);

-- administrator 的菜单项
INSERT INTO menu(`id`,`label`,`name`,`parent`, `ancestor`) VALUES 
  (10, 'console', '控制台', 1, '1'),
  (11, 'manage', '管理', 1, '1'),
  (101, 'index', '首页', 10, '1,10'),
  (102, 'userCenter', '个人中心', 10, '1,10'),
  (103, 'logout', '登出', 10, '1,10'),
  (110, 'article', '文章', 11, '1,11'),
  (111, 'user', '用户', 11, '1,11'),
  (112, 'category', '分类', 11, '1,11'),
  (113, 'tag', '标签', 11, '1,11');

-- editor 的菜单项
INSERT INTO menu(`id`,`label`,`name`,`parent`, `ancestor`) VALUES 
  (20, 'console', '控制台', 2, '2'),
  (21, 'manage', '管理', 2, '2'),
  (200, 'index', '首页', 20, '2,20'),
  (201, 'userCenter', '个人中心', 20, '2,20'),
  (202, 'logout', '登出', 20, '2,20'),
  (210, 'article', '文章', 21, '2,21'),
  (211, 'category', '分类', 21, '2,21'),
  (212, 'tag', '标签', 21, '2,21');

-- contributor 的菜单项
INSERT INTO menu(`id`,`label`,`name`,`parent`, `ancestor`) VALUES 
  (30, 'console', '控制台', 3, '3'),
  (31, 'manage', '管理', 3, '3'),
  (300, 'index', '首页', 30, '3,30'),
  (301, 'userCenter', '个人中心', 30, '3,30'),
  (302, 'logout', '登出', 30, '3,30'),
  (310, 'article', '文章', 31, '3,31');

-- subscriber 的菜单项
INSERT INTO menu(`id`,`label`,`name`,`parent`, `ancestor`) VALUES 
  (40, 'console', '控制台', 4, '4'),
  (400, 'index', '首页', 40, '4,40'),
  (401, 'userCenter', '个人中心', 40, '4,40'),
  (402, 'logout', '登出', 40, '4,40');