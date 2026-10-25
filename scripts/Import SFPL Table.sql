-- I am creating a raw table with all the data imported from the table so then I can create new tables from the raw table.

-- Select the table from file path
LOAD DATA LOCAL INFILE 'C:/Users/G_ort/Desktop/SFPL.csv'
-- Create table
INTO TABLE raw_all_info
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;
