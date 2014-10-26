{\rtf1\ansi\ansicpg1252\cocoartf1343\cocoasubrtf140
{\fonttbl\f0\fnil\fcharset0 Menlo-Regular;\f1\fnil\fcharset0 Menlo-Bold;}
{\colortbl;\red255\green255\blue255;\red46\green174\blue187;\red47\green180\blue29;\red180\green36\blue25;
}
\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural

\f0\fs22 \cf0 \CocoaLigature0 \

\f1\b \cf2 import
\f0\b0 \cf0  subprocess\

\f1\b \cf2 import
\f0\b0 \cf0  smtplib\

\f1\b \cf2 import
\f0\b0 \cf0  socket\

\f1\b \cf2 from
\f0\b0 \cf0  email.mime.text 
\f1\b \cf2 import
\f0\b0 \cf0  MIMEText\

\f1\b \cf2 import
\f0\b0 \cf0  datetime\

\f1\b \cf2 import
\f0\b0 \cf0  RPi.GPIO 
\f1\b \cf2 as
\f0\b0 \cf0  GPIO\

\f1\b \cf2 import
\f0\b0 \cf0  time\
\
GPIO.setmode(GPIO.BCM)\
\
GPIO.setup(25, GPIO.IN, pull_up_down=GPIO.PUD_UP)\
\

\f1\b \cf2 while
\f0\b0 \cf0  True:\
    input_state = GPIO.input(25)\
    
\f1\b \cf2 if
\f0\b0 \cf0  input_state == False:\
        
\f1\b \cf2 print
\f0\b0 \cf0 (
\f1\b \cf3 'Button Pressed'
\f0\b0 \cf0 )\
        time.sleep(0.2)\
        
\f1\b \cf4 # Change to your own account information
\f0\b0 \cf0 \
        to = 
\f1\b \cf3 'trigger@recipe.ifttt.com '
\f0\b0 \cf0 \
        gmail_user = 
\f1\b \cf3 'imanojsharma.one@gmail.com'
\f0\b0 \cf0 \
        gmail_password = 
\f1\b \cf3 'Arduino01'
\f0\b0 \cf0 \
        smtpserver = smtplib.SMTP(
\f1\b \cf3 'smtp.gmail.com'
\f0\b0 \cf0 , 587)\
        smtpserver.ehlo()\
        smtpserver.starttls()\
        smtpserver.ehlo\
        smtpserver.login(gmail_user, gmail_password)\
        today = datetime.date.today()\
        
\f1\b \cf4 # Very Linux Specific
\f0\b0 \cf0 \
        arg=
\f1\b \cf3 'ip route list'
\f0\b0 \cf0 \
        p=subprocess.Popen(arg,shell=True,stdout=subprocess.PIPE)\
        data = p.communicate()\
        split_data = data[0].split()\
        ipaddr = split_data[split_data.index(
\f1\b \cf3 'src'
\f0\b0 \cf0 )+1]\
        my_ip = 
\f1\b \cf3 'Your ip is %s'\cf4 # %  ipaddr
\f0\b0 \cf0 \
        msg = MIMEText(my_ip)\
        msg[
\f1\b \cf3 'Subject'
\f0\b0 \cf0 ] = 
\f1\b \cf3 'Alert Worker 1'
\f0\b0 \cf0  
\f1\b \cf4 #% today.strftime('%b %d %Y')
\f0\b0 \cf0 \
        msg[
\f1\b \cf3 'From'
\f0\b0 \cf0 ] = gmail_user\
        msg[
\f1\b \cf3 'To'
\f0\b0 \cf0 ] = to\
        smtpserver.sendmail(gmail_user, [to], msg.as_string())\
        smtpserver.quit()\
        time.sleep(20)\
\
}