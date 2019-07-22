#pragma once

// client.dll + offset /////////////////////////////////////////////////

#define mHP 0x76ACC8										 /* HP    */
#define mArmor 0x24D0C8E4									 /* Armor */

#define mHEammo 0x766318									 /* HE    */
#define mSmokeAmmo 0x766320									 /* Smoke */
#define mFlashAmmo 0x6F0648									 /* Flash */

#define mX 0x9CCFEC											 /* X     */
#define mY 0x9CCFF4											 /* Y     */
#define mZ 0x9CCFF0											 /* Z     */

////////////////////////////////////////////////////////////////////////

/* Random Stuff: */
//gameoverlayrenderer.dll
#define mFPS 0x1024A8										 /* FPS   */

////////////////////////////////////////////////////////////////////////










/* OUTDATED */
//// client.dll + offset ///////////////////////////////////////////////
//#define mHP 0x763714				 //hp
//#define mAP 0x7652EC				 //armor
//#define mKills 0x9CACE8				 //kills
//#define mDeaths 0x9CACEC			 //deaths
//#define mRifleAmmo 0x740CF4			 //drill 
//#define mRifleAmmo2 0x75EEA8		 //drill
//#define mPistolAmmo 0x742ACC		 //deagle crimson hunter
//#define mPistolAmmo2 0x75EEB4		 //deagle crimson hunter
//#define mHEammo 0x75ED70			 //HE
//#define mSmokeAmmo 0x75ED78			 //smoke
//#define mFlashAmmo 0x75ED74			 //flash
//#define bIsGameWindowActive 0x6FF7E0 //2nd way: vgui2.dll+854C4
//// hw.dll + offset ///////////////////////////////////////////////////
//#define mPing 0x2D201CC				 //ping
//#define mName 0x25B7470				 //name
//#define mX 0x1A7BF60				 //X
//#define mY 0x1A7BF68				 //Y
//#define mZ 0x1A7BF64				 //Z
//// test offsets //////////////////////////////////////////////////////
//#define lPlayerBase 0x762198 
//												   // Local Player: //
//									 /*Client + lPlayerBase + 4 + 30*/
//							     /*Client + lPlayerBase + 4 + 4 + 20*/
//								/*Client + lPlayerBase + 4 + 14 + 10*/
//	           /*Client + lPlayerBase + 4 + 24 + 0 (my favorite one)*/
////////////////////////////////////////////////////////////////////////