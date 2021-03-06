// ==========================================================
// Project: SpotiHole
// 
// Component: SpotiHole.dll
//
// Purpose: Adblocker
//          
// Initial author: meik97
//
// Started: 18.06.2020
// ==========================================================

#include "..\include\BaseInclude.hpp"

namespace Modules
{
	const char* Adblock::currentTrack;
	std::string Adblock::savedSong;
	bool Adblock::isAd;

	bool __fastcall Adblock::EnableSkips_hk(void* _this, DWORD edx)
	{
		return true;
	}

	std::string Adblock::GetCurrentTrack()
	{
		if (currentTrack == nullptr && savedSong.empty() == false)
		{
			return "Current track is not set yet";
		}
		return savedSong;
	}

	void __declspec(naked) __fastcall Adblock::SetCurrentTrack_stub(void* _this, DWORD edx, int a2, int a3, __int64 speed, int normalization, int urgency, int flag, int a8, int stream_type)
	{
		__asm
		{
			mov	    currentTrack, eax
			call    IsAdOrTrack
		}

		savedSong = currentTrack;

		__asm
		{
			push    eax
			push    131FFF0h
			push    10Eh
			push    131FFA8h
			push    128989Ah
			push    0
			push    4
			push	0CB615Ch
			retn
		}
	}

	void Adblock::IsAdOrTrack()
	{
		if (strstr(currentTrack, "spotify:track:") == nullptr && strstr(currentTrack, "spotify:episode:") == nullptr)
		{
			Utils::Utils::DebugPrint(currentTrack);
			SkipAd();
		}
		else
		{
			Utils::Utils::DebugPrint(currentTrack);
		}
	}

	void Adblock::SkipAd()
	{
		keybd_event(VK_MEDIA_NEXT_TRACK, 0, 0, 0);
	}

	void __declspec(naked) __fastcall Adblock::DisableBanner_stub(void* _this, DWORD edx, int a2, int a3)
	{
		__asm
		{
			mov		[ebp - 64h], esi
			cmp     eax, [ecx + 4]
			mov     ecx, [ebp - 5Ch]
			jz		loc_77CA5E
			cmp     ecx, esi
			jz      loc_77CA5E
			push	077CA26h
			retn

			loc_77CA5E:
			push	0077CA5Eh
			retn
		}
	}

	void __declspec(naked) __fastcall Adblock::DisableVideoAds_stub(void* __this, DWORD edx, int a2, int a3)
	{
		__asm
		{
			jz      loc_7668F9
			cmp     byte ptr[ebp - 154], 0
			jnz     loc_76686C
			cmp     byte ptr[ebp - 99h], 0
			jnz     loc_7668F9

			loc_7668F9:
			push	07668F9h
			retn

			loc_76686C:
			push	076686Ch
			retn
		}
	}

	void __declspec(naked) __fastcall Adblock::DisableVideoAds_stub2(void* __this, DWORD edx, int a2, int a3)
	{
		__asm
		{
			mov     eax, [edi]
			cmp     byte ptr[eax + 3Ch], 0
			jnz     loc_7669AE
			push	0766905h
			retn

			loc_7669AE:
			push	07669AEh
			retn
		}
	}

	DWORD sub_5D3AA0 = 0x5D3AA0;
	void __declspec(naked) __fastcall Adblock::ResponseError_stub(void* __this, DWORD edx, int a2, int a3)
	{
		__asm
		{
			cmp     eax, 200
			jnl     loc_7775CC
			mov     ecx, esi
			call    sub_5D3AA0
			cmp     eax, 300
			jle     loc_7775CC
			jmp		loc_7775CC 

			loc_7775CC:
			push	07775CCh
			retn
		}
	}

	Adblock::Adblock()
	{
		Utils::Utils::DebugPrint("Applying AdBlock patch...");
		Utils::Hook::InstallJmp(Functions::IsSkippable, EnableSkips_hk);
		Utils::Hook::InstallJmp((void*)0xCB6143, SetCurrentTrack_stub);
		Utils::Hook::InstallJmp((void*)0x77CA17, DisableBanner_stub);
		Utils::Hook::InstallJmp((void*)0x766850, DisableVideoAds_stub);
		Utils::Hook::InstallJmp((void*)0x7668F9, DisableVideoAds_stub2);
		Utils::Hook::InstallJmp((void*)0x777549, ResponseError_stub);
	}
}