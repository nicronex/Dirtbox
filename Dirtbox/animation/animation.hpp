#pragma once
#include <glm/glm.hpp>
#include <cmath>

float interpolate(float a, float b, float factor)
{
	return a * (1.0f - factor) + b * factor;
}

glm::vec3 interpolate(glm::vec3 a, glm::vec3 b, float factor)
{
	return a * (1.0f - factor) + b * factor;
}


template <typename T>
class Animation
{
private:
	T _valueBegin;
	T _valueEnd;
	T _value;
	
	float _time;
	float _duration;
	bool _playing;
	bool _playForward;
	bool _valueIsEnd;

	bool _loop;
	bool _paused;
	bool _replay;
	
public:
	Animation(T valueBegin, T valueEnd, float duration)
	: _valueBegin(valueBegin), _valueEnd(valueEnd), _duration(duration)
	{
		_value = valueBegin;
		_time = 0.0f;
		_playing = false;
		_playForward = true;
		_valueIsEnd = false;
		_loop = false;
		_paused = false;
		_replay = false;
	}

	Animation(float duration)
	: _duration(duration)
	{
		_time = 0.0f;
		_playing = false;
		_playForward = true;
		_valueIsEnd = false;
		_loop = false;
		_paused = false;
		_replay = false;
	}
	
	void setBeginValue(T value)
	{
		_valueBegin = value;
	}

	void setEndValue(T value)
	{
		_valueEnd = value;
	}

	void restart()
	{
		_playing = true;
		_paused = false;
		_time = 0.0f;
	}
	
	void start()
	{
		_playing = true;
		_paused = false;
	}

	void pause()
	{
		_paused = false;
	}
	
	void stop()
	{
		_playing = false;
		_paused = false;
		_time = 0.0f;
	}

	void reverse()
	{
		_playForward = !_playForward;
	}
	
	void setReverse(bool state)
	{
		_playForward = !state;
	}

	void setTime(float time)
	{
		_time = time;
	}

	void setLoop(bool state)
	{
		_loop = state;
	}
	
	T update(T valueBegin, T valueEnd, float delta)
	{
		if(!_playing)
		{
			if(_valueIsEnd)
			{
				_value = valueEnd;
			}
			else
			{
				_value = valueBegin;
			}
			return _value;
		}

		if(!_paused)
		{
			_time += delta;
		}
		
		if(_time > _duration)
		{
			_playing = false;
			if(_loop)
			{	
				_valueIsEnd = false;
				_value = valueBegin;
				_time = 0.0f;
			}
			else
			{
				if(_playForward)
				{
					_valueIsEnd = true;
					_value = valueEnd;
				}
				else
				{
					_valueIsEnd = false;
					_value = valueBegin;
				}
				_time = _duration;
			}
			return _value;
		}

		float factor = (1.0f / _duration) * _time;
		if(_loop)
		{
			factor = 1.0f - std::abs(factor * 2.0f - 1.0f);
		}
		else
		{
			if(!_playForward)
			{
				factor = 1.0f - factor;
			}
		}

		_value = interpolate(valueBegin, valueEnd, factor);
	
		return _value;
	}
	
	float getTime()
	{
		return _time;
	}
	
	T getValue()
	{
		return _value;	
	}

	bool isPlaying()
	{
		return _playing;
	}

	bool isReversed()
	{
		return !_playForward;
	}
};
	
