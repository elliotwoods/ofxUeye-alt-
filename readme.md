# Introduction

Capture frames from a uEye camera device

# Other ofxUeye

Paulo Barcelos created a nifty ofxUeye at 
https://github.com/paulobarcelos/ofxUeye/

I suggest checking there first.

Paulo's implementation of uEye is much more complete than mine. But doesn't handle multiple cameras (as far as I can see).

Here i'm designing for structured light capture (non-motion), so things like continuous capture and binning aren't really of interest to me.

# Does not support

* Continous capture (this requires you to work with windows events which make my skin crawl)
* Triggering (although I really wwant this, again I'd need to work with MFC windows events)
* Binning
* AOI

# Not from author

I'd have to say that IDS did a superbly awful job of making an API. They really need:
1. A C++ API (if currently you need to use the windows mfc api, then this shouldn't be a problem).
2. To get rid of functions that perform multiple tasks (e.g. get and set in one function??)
3. Proper exceptions
4. Better documentation, cleaner samples (lots of console samples please!)