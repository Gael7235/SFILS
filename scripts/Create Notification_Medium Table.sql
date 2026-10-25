-- Create NOTIFICATION_MEDIUM(notification_medium_code(PK), notification_code_definition)
CREATE TABLE notification_medium
(
	notification_medium_code			char(5)		PRIMARY KEY,
    notification_code_definition 		varchar(20)
);

-- Insert the unique values from the raw table into the notification_medium table we just created
INSERT INTO notification_medium
(
	notification_medium_code,
    notification_code_definition
)
SELECT DISTINCT
	notification_medium_code,
    notification_code_definition
FROM raw_all_info;
