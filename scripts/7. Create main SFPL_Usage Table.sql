-- Create the table SFPL_Usage(id(PK), patron_type_code(FK), patron_age_range, checkout_total, renewal_total, home_library_code(FK), notification_medium_code(FK)
--                             provided_email_address, circulation_active_year, circulation_active_month, within_SF_County, year_patron_registered)
CREATE TABLE SFPL_Usage
(
  -- Create a surrogate key
	id							int					AUTO_INCREMENT	PRIMARY KEY,
    patron_type_code			int					,
    patron_age_range			varchar(50)			,
    
    checkout_total				int					,
    renewal_total				int					,
    
    home_library_code			char(5)				,
    notification_medium_code	char(5)				,
    provided_email_address		boolean				,
    
    circulation_active_year		year				,
    circulation_active_month	varchar(20)			,
    
    within_SF_County			boolean				,
    
    year_patron_registered		year				,

  -- Create the foreign keys for the code definitions
    FOREIGN KEY(patron_type_code)
		REFERENCES patron_type(patron_type_code),
	FOREIGN KEY(home_library_code)
		REFERENCES home_library(home_library_code),
	FOREIGN KEY(notification_medium_code)
		REFERENCES notification_medium(notification_medium_code)
);

-- Insert the corresponding values from the raw table
INSERT INTO SFPL_Usage
(
	patron_type_code,
    patron_age_range,
    checkout_total,
    renewal_total,
    home_library_code,
    notification_medium_code,
    provided_email_address,
    circulation_active_year,
    circulation_active_month,
    within_SF_County,
    year_patron_registered
)
SELECT DISTINCT
	patron_type_code,
    age_range,
    checkout_total,
    renewal_total,
    home_library_code,
    notification_medium_code,
  -- provided_email_address = true if user has provided email
    CASE
		WHEN notification_medium_code = 'z'
        THEN TRUE
        ELSE FALSE
	END AS provided_email_address,
  -- These were giving me problems so I just nulled them if they were empty
    NULLIF(circulation_active_year, ''),
    NULLIF(circulation_active_month, ''),
  -- This was also giving me a problem
    CASE
		WHEN within_san_francisco_county = 'TRUE'
        THEN TRUE
        ELSE FALSE
    END AS within_san_francisco_county,
    year_patron_registered
FROM raw_all_info;
