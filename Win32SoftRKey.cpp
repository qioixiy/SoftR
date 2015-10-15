#include "SoftRApp.h"
#include "InputManager.h"
#include "Input.h"
#include "Uitilities.h"

//按下瞬间触发
void SoftRApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	//16 for middle 1 for left 2 for right
	int bits = g_input_manager->get_last_key_info()->key_bit;
	int bits_c = g_input_manager->get_last_key_info()[1].key_bit;
	bits |= btnState;
	switch (btnState)
	{
	case WIP_MOUSE_LBUTTON:
		lmouse_keep_going = true;
		break;
	case WIP_MOUSE_RBUTTON:
		rmouse_keep_going = true;
		break;
	case WIP_MOUSE_MBUTTON:
		mmouse_keep_going = true;
		break;
	}
	g_input_manager->update(bits_c, bits);
}

void SoftRApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	int bits = g_input_manager->get_last_key_info()->key_bit;
	int bits_c = g_input_manager->get_last_key_info()[1].key_bit;
	bits &= ~btnState;
	switch (btnState)
	{
	case WIP_MOUSE_LBUTTON:
		lmouse_keep_going = false;
		break;
	case WIP_MOUSE_RBUTTON:
		rmouse_keep_going = false;
		break;
	case WIP_MOUSE_MBUTTON:
		mmouse_keep_going = false;
		break;
	}

	g_input_manager->update(bits_c, bits);
}

void SoftRApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	g_input_manager->update_mouse((int)(short)x, (int)(short)y);
	g_input_manager->set_move(true);
}

void SoftRApp::OnKeyDown(WPARAM btnState)
{
	int bits = g_input_manager->get_last_key_info()->key_bit;
	int bits_c = g_input_manager->get_last_key_info()[1].key_bit;
	switch (btnState)
	{
	case 'A':bits_c |= WIP_A;
		break;
	case 'B':bits_c |= WIP_B;
		break;
	case 'C':bits_c |= WIP_C;
		break;
	case 'D':bits_c |= WIP_D;
		break;
	case 'E':bits_c |= WIP_E;
		break;
	case 'F':bits_c |= WIP_F;
		break;
	case 'G':bits_c |= WIP_G;
		break;
	case 'H':bits_c |= WIP_H;
		break;
	case 'I':bits_c |= WIP_I;
		break;
	case 'J':bits_c |= WIP_J;
		break;
	case 'K':bits_c |= WIP_K;
		break;
	case 'L':bits_c |= WIP_L;
		break;
	case 'M':bits_c |= WIP_M;
		break;
	case 'N':bits_c |= WIP_N;
		break;
	case 'O':bits_c |= WIP_O;
		break;
	case 'P':bits_c |= WIP_P;
		break;
	case 'Q':bits_c |= WIP_Q;
		break;
	case 'R':bits_c |= WIP_R;
		break;
	case 'S':bits_c |= WIP_S;
		break;
	case 'T':bits_c |= WIP_T;
		break;
	case 'U':bits_c |= WIP_U;
		break;
	case 'V':bits_c |= WIP_V;
		break;
	case 'W':bits_c |= WIP_W;
		break;
	case 'X':bits_c |= WIP_X;
		break;
	case 'Y':bits_c |= WIP_Y;
		break;
	case 'Z':bits_c |= WIP_Z;
		break;
	}
	g_input_manager->update(bits_c, bits);
}

void SoftRApp::OnKeyUp(WPARAM btnState)
{
	int bits = g_input_manager->get_last_key_info()->key_bit;
	int bits_c = g_input_manager->get_last_key_info()[1].key_bit;
	switch (btnState)
	{
	case 'A':bits_c &= ~WIP_A;
		break;
	case 'B':bits_c &= ~WIP_B;
		break;
	case 'C':bits_c &= ~WIP_C;
		break;
	case 'D':bits_c &= ~WIP_D;
		break;
	case 'E':bits_c &= ~WIP_E;
		break;
	case 'F':bits_c &= ~WIP_F;
		break;
	case 'G':bits_c &= ~WIP_G;
		break;
	case 'H':bits_c &= ~WIP_H;
		break;
	case 'I':bits_c &= ~WIP_I;
		break;
	case 'J':bits_c &= ~WIP_J;
		break;
	case 'K':bits_c &= ~WIP_K;
		break;
	case 'L':bits_c &= ~WIP_L;
		break;
	case 'M':bits_c &= ~WIP_M;
		break;
	case 'N':bits_c &= ~WIP_N;
		break;
	case 'O':bits_c &= ~WIP_O;
		break;
	case 'P':bits_c &= ~WIP_P;
		break;
	case 'Q':bits_c &= ~WIP_Q;
		break;
	case 'R':bits_c &= ~WIP_R;
		break;
	case 'S':bits_c &= ~WIP_S;
		break;
	case 'T':bits_c &= ~WIP_T;
		break;
	case 'U':bits_c &= ~WIP_U;
		break;
	case 'V':bits_c &= ~WIP_V;
		break;
	case 'W':bits_c &= ~WIP_W;
		break;
	case 'X':bits_c &= ~WIP_X;
		break;
	case 'Y':bits_c &= ~WIP_Y;
		break;
	case 'Z':bits_c &= ~WIP_Z;
		break;
	}
	g_input_manager->update(bits_c, bits);
}
