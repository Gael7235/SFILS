-- Create PATRON_TYPE(patron_type_code(PK), patron_type_definition)
CREATE TABLE patron_type
(
	patron_type_code		int		PRIMARY KEY,
  patron_type_definition	varchar(50)
);

-- Insert the unique values from the raw table into the patron_type table we just created
INSERT INTO patron_type
(
	patron_type_code,
  patron_type_definition
)
SELECT DISTINCT
  patron_type_code,
  patron_type_definition
FROM raw_all_info;
