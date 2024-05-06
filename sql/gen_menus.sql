-- 每个角色的顶级菜单
INSERT INTO menu(`id`,`key`,`label`,`parent`) VALUES 
  (1, 'administrator', 'administrator', NULL),
  (2, 'editor', 'editor', NULL),
  (3, 'contributor', 'contributor', NULL),
  (4, 'subscriber', 'subscriber', NULL),
  (5, 'visitor', 'visitor', NULL);

-- administrator 的菜单项
INSERT INTO menu(`id`,`key`,`label`,`parent`) VALUES 
  (6, 'console', '控制台', 1),
  (7, 'manage', '管理', 1),
  (8, 'index', '首页', 1),
  (9, 'logout', '登出', 1),
  (10, 'userCenter', '个人中心', 6),
  (11, 'article', '文章', 7),
  (12, 'user', '用户', 7),
  (13, 'category', '分类', 7),
  (14, 'tag', '标签', 7);

-- editor 的菜单项
INSERT INTO menu(`id`,`key`,`label`,`parent`) VALUES 
  (15, 'console', '控制台', 2),
  (16, 'manage', '管理', 2),
  (17, 'index', '首页', 2),
  (18, 'logout', '登出', 2),
  (19, 'userCenter', '个人中心', 15),
  (20, 'article', '文章', 16),
  (21, 'category', '分类', 16),
  (22, 'tag', '标签', 16);

-- contributor 的菜单项
INSERT INTO menu(`id`,`key`,`label`,`parent`) VALUES 
  (23, 'console', '控制台', 3),
  (24, 'manage', '管理', 3),
  (25, 'index', '首页', 3),
  (26, 'logout', '登出', 3),
  (27, 'userCenter', '个人中心', 23),
  (28, 'article', '文章', 24);

-- subscriber 的菜单项
INSERT INTO menu(`id`,`key`,`label`,`parent`) VALUES 
  (29, 'console', '控制台', 4),
  (30, 'index', '首页', 4),
  (31, 'logout', '登出', 4),
  (32, 'userCenter', '个人中心', 29);