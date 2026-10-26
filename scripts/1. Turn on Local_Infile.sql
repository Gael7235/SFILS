-- This is for a much easier import from the excel sheet database. I didn't have much luck in using the integrated table data import wizard in mysql as it was barely making progress in importing the data.

-- Set the Local_Infile value to 1 or ON
SET GLOBAL LOCAL_INFILE = 1;
-- Check to see if it worked
SHOW VARIABLES LIKE 'local_infile';

--Shown in the Screenshots in the docs repository (Labeled 1.a) turn on Local_Infile=1.
--Have to enable it in the server connections for your specific server using OPT_LOCAL_INFILE=1 (shown in Screenshot 1.b in docs)
--NOTE: Only works for MySQL version 8.0.4+
