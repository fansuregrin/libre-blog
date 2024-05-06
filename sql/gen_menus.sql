-- 每个角色的顶级菜单
INSERT INTO menu(`id`,`key`,`label`,`parent`) VALUES 
  (1, 'administrator', 'administrator', NULL),
  (2, 'editor', 'editor', NULL),
  (3, 'contributor', 'contributor', NULL),
  (4, 'subscriber', 'subscriber', NULL);

-- administrator 的菜单项
INSERT INTO menu(`id`,`key`,`label`,`parent`) VALUES 
  (5, 'console', '控制台', 1),
  (6, 'manage', '管理', 1),
  (7, 'index', '首页', 1),
  (8, 'logout', '登出', 1),
  (9, 'userCenter', '个人中心', 5),
  (10, 'article', '文章', 6),
  (11, 'user', '用户', 6),
  (12, 'category', '分类', 6),
  (13, 'tag', '标签', 6);

-- editor 的菜单项
INSERT INTO menu(`id`,`key`,`label`,`parent`) VALUES 
  (14, 'console', '控制台', 2),
  (15, 'manage', '管理', 2),
  (16, 'index', '首页', 2),
  (17, 'logout', '登出', 2),
  (18, 'userCenter', '个人中心', 14),
  (19, 'article', '文章', 15),
  (20, 'category', '分类', 15),
  (21, 'tag', '标签', 15);

-- contributor 的菜单项
INSERT INTO menu(`id`,`key`,`label`,`parent`) VALUES 
  (22, 'console', '控制台', 3),
  (23, 'manage', '管理', 3),
  (24, 'index', '首页', 3),
  (25, 'logout', '登出', 3),
  (26, 'userCenter', '个人中心', 22),
  (27, 'article', '文章', 23);

-- subscriber 的菜单项
INSERT INTO menu(`id`,`key`,`label`,`parent`) VALUES 
  (28, 'console', '控制台', 4),
  (29, 'index', '首页', 4),
  (30, 'logout', '登出', 4),
  (31, 'userCenter', '个人中心', 28);