# smoke_alarm.py - send email every 5 minutes when smoke is detected
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import botbook_mcp3002 as mcp
import smtplib	# <1>
from email.mime.text import MIMEText	# <2>

# Email addresses
email_to = 'example@gmail.com'	# <3>
email_from = 'example@gmail.com'	# <4>

# SMTP email server settings
server = 'smtp.gmail.com'	# <5>
mail_port = 587  
user = 'example@gmail.com'	# <6>
password = 'password'	# <7>
gracePeriod = 5 * 60 # seconds	# <8>

def sendEmail(subject, msg):	# <9>
	msg = MIMEText(msg)	# <10>
	msg['Subject'] = subject	# <11>
	msg['To'] = email_to	# <12>
	msg['From'] = email_from

	smtp = smtplib.SMTP(server,mail_port)	# <13>
	smtp.starttls() 
	smtp.login(user, password)	# <14>
	smtp.sendmail(email_from, email_to, msg.as_string())	# <15>
	smtp.quit()	# <16>

def main():
	while True:
		smokeLevel = mcp.readAnalog()
		print("Current smoke level is %i " % smokeLevel)
		if smokeLevel > 120:
			print("Smoke detected")
			sendEmail("Smoke","Smoke level was %i" % smokeLevel) # <17>
			time.sleep(gracePeriod)	# <18>
		time.sleep(0.5) # s	# <19>

if __name__ == "__main__":
	main()




