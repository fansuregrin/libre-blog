SELECT
    art.id,
    art.title,
    user.username as author_name,
    cat.name as category_name,
    art.content
FROM article art
JOIN user ON art.author = user.id
JOIN category cat ON art.category = cat.id
WHERE art.id = 3;