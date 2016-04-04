========
webBot++
========

About:
======

	* This is an attempt to port the webBot package I have written in PHP to C++, it is currently in development and does not work at all. If you feel like helping to contribute on it feel free to branch the repo and i'll merge any additions that look good :)

	* The first version does not aim to include any curl_multi_* features, they will be added in later versions but for now I want to KISS

	* If anyones interested the curl handle appears to be getting NULLed out between the constructor and the call to requestGET() in main, copying and pasting code from requestGET to the constructor of HTTPBot works so it seems somewhere between things its getting nulled... curious...