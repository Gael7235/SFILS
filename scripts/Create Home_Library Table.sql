-- Create HOME_LIBRARY(home_library_code(PK), Home_library_definition)
CREATE TABLE home_library
(
	home_library_code			char(5)		PRIMARY KEY,
    home_library_definition 	varchar(50)
);

-- Insert the unique values from the raw table into the home_library table we just created
INSERT INTO home_library
(
	home_library_code,
    home_library_definition
)
SELECT DISTINCT
	home_library_code,
    home_library_definition
FROM raw_all_info;
