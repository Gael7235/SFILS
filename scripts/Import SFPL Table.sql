-- I am creating a raw table with all the data imported from the table so then I can create new tables from the raw table.

-- Create the table to store all data
CREATE TABLE RAW_ALL_INFO
(
patron_type_code 				      varchar(100) 	NOT NULL,
patron_type_definition 			  varchar(100) 	NOT NULL,
checkout_total 					      varchar(100) 	DEFAULT(0),
renewal_total 					      varchar(100) 	DEFAULT(0),
age_range 						        varchar(100) 	NOT NULL,
home_library_code 				    varchar(100) 	NOT NULL,
home_library_definition 		  varchar(100) 	NOT NULL,
circulation_active_month 		  varchar(100) 	NULL,
circulation_active_year 		  varchar(100) 	NULL,
notification_medium_code 		  varchar(100) 	NOT NULL,
notification_code_definition 	varchar(100) 	NOT NULL,
provided_email_address 			  varchar(100) 	NOT NULL,
within_san_francisco_county 	varchar(100) 	NOT NULL,
year_patron_registered			  varchar(100) 	NOT NULL
);

-- Select the table from file path
LOAD DATA LOCAL INFILE 'C:/Users/G_ort/Desktop/SFPL.csv'
-- Import into the raw table
INTO TABLE raw_all_info
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;
