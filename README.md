# pcsx2-1.4.0-rr
This README proposes a translation (and an update of certain parts, of course) of the original README (be wary that it may not be accurate as I don't speak nor understand Japanese and most of the translation comes from Google Translation). Consider this project as version 2 of PCSX2-rr.<br>
This informal emulator adds features needed to create a TAS to [pcsx2-1.4.0](https://github.com/PCSX2/pcsx2).<br>
This is inspired by pcsx2-rr(<https://code.google.com/archive/p/pcsx2-rr/>). However, since the content has changed a lot and simple merging can not be done, the source code has changed considerably.
  

# TAS
Here are some TAS examples (made with the original pcsx2-rr v.1):
* [�yTAS�z�`���E���b�v part0(WIP)�ypcsx2-1.4.0-rr�z(�j�R�j�R)](http://www.nicovideo.jp/watch/sm30385451)  
* [�yTAS�z�`���E���b�v part0(WIP)�ypcsx2-1.4.0-rr�z(youtube)](https://youtu.be/Ib2MnRfCCzc)  


# About desync
See this [wiki page](https://github.com/pocokhc/pcsx2-1.4.0-rr/wiki#%E3%82%AD%E3%83%BC%E3%83%A0%E3%83%BC%E3%83%93%E3%83%BC%E4%BD%9C%E6%88%90%E6%83%B3%E5%AE%9A%E6%89%8B%E9%A0%86201749%E6%9B%B4%E6%96%B0) (in Japanese).

# Download
You can download the Windows 10 executable from [here](TODO)
  
You will need the followings to run the executable:
* Visual C++ 2015 x86 Redistributable. Download it [HERE](https://www.microsoft.com/en-us/download/details.aspx?id=48145)  
  
This was tested only on Windows 10, but the application should work on the other OS supported by PCSX2.

# Added features
* KeyMovie recording
* Pause/Unpause/FrameAdvance
* Lua (this version does not close the Lua engine when the end of the file is reached)
* An editor for KeyMovie files
  
For more details, see the wikis:
* [PCSX2-rr v1](https://github.com/pocokhc/pcsx2-1.4.0-rr/wiki) (in Japanese)
* [PCSX2-rr v2](https://github.com/DocSkellington/pcsx2-1.4.0-rr/wiki) (only explains changes)



# How to build (Windows 10)
You can build this version as you would build the original PCSX2. It is easier with Visual Studio 2015 (works with 2017 version as well).
  
�p�ӂ�����  
1. Visual Studio Comunnity 2015(<https://www.visualstudio.com/vs/community/>)  
�@���f�t�H���g�ݒ肾��C++������Ȃ��̂ŃJ�X�^�����v���O���~���O���ꁨVisual C++�Ƀ`�F�b�N�����ăC���X�g�[�����Ă��������B  
2. DirectX Software Development Kit (June 2010)(<https://www.microsoft.com/en-us/download/details.aspx?id=6812>)  
�@���f�t�H���g�ݒ�̃C���X�g�[���ł����ł��B�ǉ���.NET Framework3.5���K�v�Ȃ̂Œǉ����܂����H�ƕ������ꍇ�̓C���X�g�[�����Ă����܂��B  
3. �\�[�X�R�[�h��́uPCSX2_suite.sln�v��J���A�r���h��܂��B  
  
�_�E�����[�h����уr���h�Ɋւ��Ă�[windows10](https://www.microsoft.com/ja-jp/software-download/windows10)��VM��ŐV�K�ɃC���X�g�[��������ԂŋN���ł������Ƃ�m�F���Ă��܂��B  


# �\�[�X�R�[�h�̕ύX�ӏ�
���̃R�[�h����ύX���Ă���ӏ��ɂ͂��ׂĉ��L�R�����g�������Ă��܂��B  
//--TAS--//  
//--LuaEngine--//  
//--TASKeyEditor--//  
  
�܂��A�V�K�ǉ������t�@�C���͈ȉ��f�B���N�g���ł��B  
./pcsx2/TAS/  
./pcsx2/lua/  
./3rdparty/lua/  
  

# �R�����g
�ЂƂ܂�v3.0�ŊJ���͎~�߂�TAS����Ɉڂ肽���Ǝv���܂��B  
TAS��쐬���Ă��������ŋC�ɂȂ����_��o�O���o��ΐ����Ή��͂��Ă����\��ł��B  
����Lua�͂��܂�e�X�g���Ă��Ȃ��̂Łc�A�o�O���o���Lua�̎g�p��T���Ă�炦���(����  



------------------------
# Licence
Same as PCSX2: [License GNU LGPL](http://www.gnu.org/licenses/lgpl.html)  

