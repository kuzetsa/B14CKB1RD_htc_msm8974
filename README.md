B14CKB1RD M8 Kernel 
==========================

Kernel for HTC msm8974 devices

### THIS IS NOT YET STABLE!!! ###
* As of 2015 July 22nd
* **Use at your own risk**
* Purpose of this fork is porting to Android 5.1.1<br>(*API Level 22 is newer than Level 21 AKA android 5.0, both are called lollipop*)

---

#### Following status list is from **PRE-FORK** state ####
(*status is unclear which, if any of these items are stable, partially working, implemented at all, etc.*)

```
Changelog for upcoming build
* 2.57 OC
* 268 UC
* 600, 625, 650 steps for gpu, start at 200 mhz
* slimbus overclock maxed out
* fauxsound
* enable all sleep states
* multicore powersaving defaulted to 2
* power efficient wq
* fsync
* fastcharge
* frandom
* simple gpu algorithm
* faux powersuspend, you need to set in in fauxclock
* optimized arm crypto
* Faux's nexus 5 ondemand, gtfo htc ondemand
* moto memory crack! *snorts*
* optimized simple ondemand gpu gov
* krait topology
* moto long list reading improvments
* optimized deadline, optimized sio, bfq 7v7 with eqm, fiops and zen
* optimized rwsem
* kernel mode neon
* entropy tweaks
* bacon tweaks!!
* advanced tcp, westwood set to default
* multirom support
* dynamic page writeback (enjoy the extra battery)
* a15 optimizations from nvidia
* intelliplug 3.8
* and vibration control (though i haven't tested this feature, and idk which apps support the one i added)
```
