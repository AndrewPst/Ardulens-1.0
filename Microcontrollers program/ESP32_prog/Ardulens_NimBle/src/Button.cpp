#include <Button.h>

#define FILTER_TIME 40
#define MULTICLICK_PAUSE_TIME 240
#define HOLD_TIME 260

// ==================== CONSTRUCTOR ====================

TouchButton::TouchButton(uint8_t pin) : pin_(pin)
{
}

//===================== LOGIC ==========================

//===================== GETTER =========================

bool TouchButton::isPressed()
{
	return lastPressState;
}

ButtonAction_t TouchButton::getButtonAction()
{
	return lastType_;
}

uint16_t TouchButton::getClicksCount()
{
	return clickCounter_;
}

bool TouchButton::isNewAction()
{
	return isNewAction_;
}

bool TouchButton::isStateChanged()
{
	return isStateChanged_;
}

void TouchButton::tick()
{
	bool state = touchRead(pin_) < 40;
	uint32_t now = millis();
	if (state != lastTouchState_)
	{
		if (state && !lastTouchState_)
		{
			lastTouchTime_ = now;
		}
		else if (!state && lastTouchState_)
		{
			uint16_t dif = now - lastTouchTime_;
			if (dif > HOLD_TIME)
			{
				actionType = B_HOLD;
				multiclickFlag_ = false;
			}
			if (now - lastTouchTime_ > FILTER_TIME)
			{
				releaseTime_ = now;
				if (actionType != B_HOLD)
				{
					if (!multiclickFlag_)
						clickCounter_ = 1;
					else
						clickCounter_++;
					multiclickFlag_ = true;
					actionType = B_CLICK;
				}
			}
		}
		lastTouchState_ = state;
	}
	uint16_t dif = now - lastTouchTime_;
	if (now - releaseTime_ > MULTICLICK_PAUSE_TIME && !state && multiclickFlag_)
	{
		actionType = B_MULTICLICK;
		multiclickFlag_ = false;
	}
	if (dif > FILTER_TIME && dif < HOLD_TIME && state)
	{
		actionType = B_PRESSED;
	}
	if (dif > HOLD_TIME && state)
	{
		actionType = B_HOLDED;
	}
	if (actionType != lastType_)
	{
		bool isPress = actionType == B_PRESSED || actionType == B_HOLDED;
		if (isPress != lastPressState)
		{
			isStateChanged_ = true;
			lastPressState = isPress;
		}
		lastType_ = actionType;
		isNewAction_ = true;
	}
	else
	{
		isNewAction_ = false;
		isStateChanged_ = false;
	}
}