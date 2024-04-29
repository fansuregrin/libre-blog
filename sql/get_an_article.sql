SELECT
    art.id,
    art.title,
    author.name as author_name,
    cat.name as category_name,
    art.content
FROM article art
JOIN author ON art.author = author.id
JOIN category cat ON art.category = cat.id
WHERE art.id = 3;