Minesweeper-Videos-Analyzer
===========================

# English Introduction #

Using it, you can analyse the most popluar minesweeper videos(mvf, avf, mvr, rvm, fsvf), generate summaries, and export the results to an excel document easily.

To complie it, you should us Visual Studio 2013 or later version. You should copy the "core" and "lib" dir to the output dir (usually its name is Debug or Release) manually, otherwise, the decoder will not be found.

BTW, the mvr decoder my be treated as virus by some ant-virus software, so I renamed it as mvr2rawvf.ex_. You can rename it back as mvr2rawvf.exe by yourself, then it will work.

The decoder source code is from Cryslon's rawvf project. You can get the latest news of rawvf project at http://www.minesweeper.info/forum/viewtopic.php?f=26&t=86

You can get minesweeper videos at www.minesweeper.info or www.saolei.net

# ����˵�� #

ʹ�ñ�������Է����󲿷����е�ɨ��¼���ļ������������ɵĵ������������Excel�ĵ���Ŀǰ֧�ֵĸ�ʽ��mvf��avf��mvr��rvm��fsvf��

������Ҫʹ��Visual Studio 2013����µİ汾������Ҫ�ֶ���core��lib�������ļ��п���������������ļ���(����ͨ����Debug����Release)����Ȼ������Ҳ�����������

���⣬mvr��ʽ�Ľ������ᱻĳЩɱ��������������������Ұ���������Ϊ��mvr2rawvf.ex_�����������ȷ����mvr��ʽ��¼��������Լ�����������Ϊmvr2rawvf.exe��

��������Դ��������Cryslon��rawvf���̣���ȡrawvf����������Ѷ�����Է��ʣ�http://www.minesweeper.info/forum/viewtopic.php?f=26&t=86

ɨ��¼�����أ��뵽www.minesweeper.info����www.saolei.net��

# History #

2015-12-13 v1.1
* Support fsvf,the video file for Freesweeper.
* Added filter for the same video files.

2014-04-17 v1.0 (with huge improvements)
* Added build-in video parser which is faster than the .exe parser.
* Added a switch to allow using fast parser or old parser.
* Added a switch to allow scanning sub directories.
* Added version information in About Dialog.
* Fixed a bug while parsing rmv.

2014-04-11 v0.5
* Added Tail Generator.
* Adjust something about UI.

2014-04-10 v0.4
* Fixed export failure bug on Win7 + MS Office2010.
* Added directory path remember feature.
* Added competition mode.
* Added searching sub-dir for video files.

2014-03-26 v0.3
* Add some useful information while analyzing.
* Optimized the thread pool strategy for large number of files.

2014-03-25 v0.2
* Optimized the work thread according to the cpu core number.
* Fixed a crash bug if there is no player to play the video file but we double click on the file information.
* Support cancel the analyzing process.
* Automatically hide the analyzing dialog when finish the Analysis.

2014-03-25 v0.1
* Finished the 1st version, it can analyze a minesweeper video folder & export the result to an excel document.

2014-03-23
* Started this project.