/** @file
 * @brief	Bitboard Reversi �N���X
 *
 * @author	P-04148 �A�� ���V
 *
 */



#include "bitboard_reversi.h"

using namespace std;
using namespace Fujitalab::Reversi;

BitboardReversi::BitboardReversi()
{
	// �{�[�h�̃N���A
	this->black = this->white = 0;
	this->current_color = true;

	this->set( toBitboard(SIZE_X / 2 - 1, SIZE_Y / 2 - 1), true  );
	this->set( toBitboard(SIZE_X / 2 - 1, SIZE_Y / 2    ), false );
	this->set( toBitboard(SIZE_X / 2    , SIZE_Y / 2 - 1), false );
	this->set( toBitboard(SIZE_X / 2    , SIZE_Y / 2    ), true  );

}

// �ǖʂɎw�肵���F�̐΂�u��
void BitboardReversi::set(Bitboard b, bool color)
{
	if(color)
		this->black |= b;
	else
		this->white |= b;
}

// �ǖʂ�\������
void BitboardReversi::view()
{
	Bitboard _black = this->black;
	Bitboard _white = this->white;
	Bitboard mobility = this->getMobility(this->black, this->white);

	cout << endl << "[" << (this->current_color ? "*" : "o") << "]";

	for ( int i = 0 ; i < MAX_SIZE ; i ++ )
	{
		// ���s
		if ( ( i % MAX_SIZE_X == 0 ) && ( ( i / MAX_SIZE_Y ) < SIZE_Y ) )
			cout << endl;

		// ������������
		if ( ( ( i % MAX_SIZE_X ) < SIZE_X ) && ( ( i / MAX_SIZE_Y ) < SIZE_Y ) )
		{
			if ( ( _black & 0x8000000000000000ULL ) != 0 )
				cout << (this->current_color ? "* " : "o ");
			else if ( ( _white & 0x8000000000000000ULL ) != 0 )
				cout << (this->current_color ? "o " : "* ");
			else if ( ( mobility & 0x8000000000000000ULL ) != 0 )
				cout << "x ";
			else
				cout << ". ";
		}

		// ���̏ꏊ�ɐi��
		_black <<= 1;
		_white <<= 1;
		mobility <<= 1;

	}
	cout << endl;
}

// ����\�ȏꏊ��\������
void BitboardReversi::viewMobility()
{
	Bitboard mobility = this->getMobility(this->black, this->white);

	for ( int i = 0 ; i < MAX_SIZE ; i ++ )
	{
		// ���s
		if ( ( i % MAX_SIZE_X == 0 ) && ( ( i / MAX_SIZE_Y ) < SIZE_Y ) )
			cout << endl;

		if ( ( ( i % MAX_SIZE_X ) < SIZE_X ) && ( ( i / MAX_SIZE_Y ) < SIZE_Y ) )
		{
			// �u����Ƃ��������
			if ( ( mobility & 0x8000000000000000ULL ) != 0 )
				cout << "x ";
			else
				cout << ". ";
		}

		// ���̏ꏊ�ɐi��
		mobility <<= 1;

	}
	cout << endl;
}

// ���݂̃{�[�h�Œ���\�Ȏ���r�b�g�{�[�h�Ŏ擾����
Bitboard BitboardReversi::getMobility()
{
	r_Left			= getMobilityLeft(black, white);
	r_Right			= getMobilityRight(black, white);
	r_Top			= getMobilityTop(black, white);
	r_Bottom		= getMobilityBottom(black, white);
	r_LeftTop		= getMobilityLeftTop(black, white);
	r_LeftBottom	= getMobilityLeftBottom(black, white);
	r_RightTop		= getMobilityRightTop(black, white);
	r_RightBottom	= getMobilityRightBottom(black, white);

	return r_Left | r_Right | r_Top | r_Bottom | r_LeftTop | r_LeftBottom | r_RightTop | r_RightBottom;
}

// �^�����ǖʂō�������\�Ȏ���r�b�g�{�[�h�Ŏ擾����
Bitboard BitboardReversi::getMobility(Bitboard black, Bitboard white)
{
	r_Left			= getMobilityLeft(black, white);
	r_Right			= getMobilityRight(black, white);
	r_Top			= getMobilityTop(black, white);
	r_Bottom		= getMobilityBottom(black, white);
	r_LeftTop		= getMobilityLeftTop(black, white);
	r_LeftBottom	= getMobilityLeftBottom(black, white);
	r_RightTop		= getMobilityRightTop(black, white);
	r_RightBottom	= getMobilityRightBottom(black, white);

	return r_Left | r_Right | r_Top | r_Bottom | r_LeftTop | r_LeftBottom | r_RightTop | r_RightBottom;
}

// ���肵���Ƃ��ɔ��]����ӏ����r�b�g�{�[�h�Ŏ擾
Bitboard BitboardReversi::getReversedPattern(Bitboard m)
{
	Bitboard rev = 0;
	Bitboard mask;
	Bitboard _rev;

	// �����u���Ȃ��Ƃ��i�p�X�̂Ƃ��j�͉��������ɏI��
	// (�p�X�������ƌ��ʂ��o�邪�A���Ȃ��Ƌt�ɏ����ʂ͑�����)
	if ( m == 0 )
	{
		return 0;
	}

    // [TODO] ������ while ���͓W�J��������������
	if (( r_Left & m ) == m )
	{
		_rev = 0;
		mask = transferLeft(m);
		while( mask != 0 && (mask & this->white) != 0 )
		{
			_rev |= mask;
			mask = transferLeft(mask);
		}
		if( (mask & this->black) != 0 )
			rev |= _rev;
	}

	if (( r_Right & m ) == m )
	{
		_rev = 0;
		mask = transferRight(m);
		while( mask != 0 && (mask & this->white) != 0 )
		{
			_rev |= mask;
			mask = transferRight(mask);
		}
		if( (mask & this->black) != 0 )
			rev |= _rev;
	}

	if (( r_Top & m ) == m )
	{
		_rev = 0;
		mask = transferTop(m);
		while( mask != 0 && (mask & this->white) != 0 )
		{
			_rev |= mask;
			mask = transferTop(mask);
		}
		if( (mask & this->black) != 0 )
			rev |= _rev;
	}

	if (( r_Bottom & m ) == m )
	{
		_rev = 0;
		mask = transferBottom(m);
		while( mask != 0 && (mask & this->white) != 0 )
		{
			_rev |= mask;
			mask = transferBottom(mask);
		}
		if( (mask & this->black) != 0 )
			rev |= _rev;
	}

	if (( r_LeftTop & m ) == m )
	{
		_rev = 0;
		mask = transferLeftTop(m);
		while( mask != 0 && (mask & this->white) != 0 )
		{
			_rev |= mask;
			mask = transferLeftTop(mask);
		}
		if( (mask & this->black) != 0 )
			rev |= _rev;
	}

	if (( r_LeftBottom & m ) == m )
	{
		_rev = 0;
		mask = transferLeftBottom(m);
		while( mask != 0 && (mask & this->white) != 0 )
		{
			_rev |= mask;
			mask = transferLeftBottom(mask);
		}
		if( (mask & this->black) != 0 )
			rev |= _rev;
	}

	if (( r_RightTop & m ) == m )
	{
		_rev = 0;
		mask = transferRightTop(m);
		while( mask != 0 && (mask & this->white) != 0 )
		{
			_rev |= mask;
			mask = transferRightTop(mask);
		}
		if( (mask & this->black) != 0 )
			rev |= _rev;
	}

	if (( r_RightBottom & m ) == m )
	{
		_rev = 0;
		mask = transferRightBottom(m);
		while( mask != 0 && (mask & this->white) != 0 )
		{
			_rev |= mask;
			mask = transferRightBottom(mask);
		}
		if( (mask & this->black) != 0 )
			rev |= _rev;
	}

	return rev;
}


// �^������i�r�b�g�{�[�h�i�r�b�g�񒆂�1�̃r�b�g��1�����j�j�𒅎肷��
// �������A���̎�͒��肪�\�Ȃ��̂Ƃ���i���̊֐����g���O�͗\�ߒ��肪�\���𒲂ׂ�j
void BitboardReversi::put(Bitboard m)
{
	// push
	this->board_stack.push(this->toBoard());

	Bitboard rev = this->getReversedPattern(m);
	this->black ^= m | rev;
	this->white ^= rev;

	this->changed = false;

	// black <--> white
	this->black = this->black ^ this->white;
	this->white = this->white ^ this->black;
	this->black = this->black ^ this->white;

	this->current_color ^= true;
	this->changed = true;

}

// �^�������Wx,y�ɒ��肷��
// put(Bitboard m)�ƂقƂ�Ǔ����i���ԉ����������j
void BitboardReversi::put(int x, int y)
{
	this->put(toBitboard(x,y));
}

// ���߂�
void BitboardReversi::undo()
{
	Board b = this->board_stack.top();
	this->board_stack.pop();
	this->black = b.black;
	this->white = b.white;
	this->current_color = b.current_color;
	r_Left = b.left;
	r_Right = b.right;
	r_Top = b.top;
	r_Bottom = b.bottom;
	r_LeftTop = b.lefttop;
	r_LeftBottom = b.leftbottom;
	r_RightTop =  b.righttop;
	r_RightBottom = b.rightbottom;
}

// ���݂̃{�[�h�̏�Ԃ� Board �^�ɂ���
Board BitboardReversi::toBoard()
{
	Board b;

	b.black = this->black;
	b.white = this->white;
	b.current_color = this->current_color;
	b.left = r_Left;
	b.right = r_Right;
	b.top = r_Top;
	b.bottom = r_Bottom;
	b.lefttop = r_LeftTop;
	b.leftbottom = r_LeftBottom;
	b.righttop = r_RightTop;
	b.rightbottom = r_RightBottom;

	return b;
}

// ���S��put
// �������A�x��
void BitboardReversi::putSafety(int x, int y)
{
	this->getMobility();
	this->put(x,y);
}
void BitboardReversi::putSafety(Bitboard m)
{
	this->getMobility();
	this->put(m);
}

void BitboardReversi::setBoard(Bitboard black, Bitboard white, bool current_color)
{
    this->black = black;
    this->white = white;
    this->current_color = current_color;
}

