#include "Easing.h"
#include "Maths.h"

namespace Easing
{
	float QuadIn(float t, float total, float min, float max)
	{
		max -= min;
		t /= total;
		return max * t * t + min;
	}

	float QuadOut(float t, float total, float min, float max)
	{
		max -= min;
		t /= total;
		return -max * t * (t - 2.0f) + min;
	}

	float QuadInOut(float t, float total, float min, float max)
	{
		max -= min;
		t /= total / 2.0f;
		if (t < 1.0f)return max / 2.0f * t * t + min;

		t = t - 1;
		return -max / 2.0f * (t * (t - 2) - 1) + min;
	}

	float CubicIn(float t, float total, float min, float max)
	{
		max -= min;
		t /= total;
		return max * t * t * t + min;
	}

	float CubicOut(float t, float total, float min, float max)
	{
		max -= min;
		t = t / total - 1.0f;
		return max * (t * t * t + 1) + min;
	}

	float CubicInOut(float t, float total, float min, float max)
	{
		max -= min;
		t /= total / 2.0f;
		if (t < 1.0f) return max / 2.0f * t * t * t + min;

		t = t - 2.0f;
		return max / 2.0f * (t * t * t + 2.0f) + min;
	}

	float QuartIn(float t, float total, float min, float max)
	{
		max -= min;
		t /= total;
		return max * t * t * t * t + min;
	}

	float QuartOut(float t, float total, float min, float max)
	{
		max -= min;
		t = t / total - 1.0f;
		return -max * (t * t * t * t - 1.0f) + min;
	}

	float QuartInOut(float t, float total, float min, float max)
	{
		max -= min;
		t /= total / 2.0f;
		if (t < 1.0f) return max / 2.0f * t * t * t * t + min;

		t = t - 2.0f;
		return -max / 2.0f * (t * t * t * t - 2.0f) + min;
	}

	float QuintIn(float t, float total, float min, float max)
	{
		max -= min;
		t /= total;
		return max * t * t * t * t * t + min;
	}

	float QuintOut(float t, float total, float min, float max)
	{
		max -= min;
		t = t / total - 1.0f;
		return max * (t * t * t * t * t + 1.0f) + min;
	}

	float QuintInOut(float t, float total, float min, float max)
	{
		max -= min;
		t /= total / 2.0f;
		if (t < 1.0f) return max / 2.0f * t * t * t * t * t + min;

		t = t - 2.0f;
		return max / 2.0f * (t * t * t * t * t + 2.0f) + min;
	}

	float SineIn(float t, float total, float min, float max)
	{
		max -= min;
		return -max * cosf(t * (M_PI * 90.0f / 180.0f) / total) + max + min;
	}

	float SineOut(float t, float total, float min, float max)
	{
		max -= min;
		return max * sinf(t * (M_PI * 90.0f / 180.0f) / total) + max + min;
	}

	float SineInOut(float t, float total, float min, float max)
	{
		max -= min;
		return -max / 2.0f * (cosf(t * M_PI / total) - 1.0f) + min;
	}

	float ExpIn(float t, float total, float min, float max)
	{
		max -= min;
		return t == 0.0f ? min : max * powf(2.0f, 10.0f * (t / total - 1.0f)) + min;
	}

	float ExpOut(float t, float total, float min, float max)
	{
		max -= min;
		return t == total ? max + min : max * (-powf(2.0f, -10.0f * t / total) + 1.0f) + min;
	}

	float ExpInOut(float t, float total, float min, float max)
	{
		if (t == 0.0f) return min;
		if (t == total) return max;
		max -= min;
		t /= total / 2.0f;

		if (t < 1.0f) return max / 2.0f * powf(2.0f, 10.0f * (t - 1.0f)) + min;

		t = t - 1.0f;
		return max / 2.0f * (-powf(2.0f, -10.0f * t) + 2) + min;
	}

	float CircIn(float t, float total, float min, float max)
	{
		max -= min;
		t /= total;
		return -max * (sqrtf(1.0f - t * t) - 1.0f) + min;
	}

	float CircOut(float t, float total, float min, float max)
	{
		max -= min;
		t = t / total - 1.0f;
		return max * sqrtf(1.0f - t * t) + min;
	}

	float CircInOut(float t, float total, float min, float max)
	{
		max -= min;
		t /= total / 2.0f;
		if (t < 1.0f) return -max / 2.0f * (sqrtf(1.0f - t * t) - 1.0f) + min;

		t = t - 2.0f;
		return max / 2.0f * (sqrtf(1.0f - t * t) + 1.0f) + min;
	}

	float ElasticIn(float t, float total, float min, float max)
	{
		max -= min;
		t /= total;

		float s = 1.70158f;
		float p = total * 0.3f;
		float a = max;

		if (t == 0.0f) return min;
		if (t == 1.0f) return min + max;

		if (a < fabsf(max))
		{
			a = max;
			s = p / 4.0f;
		}
		else
		{
			s = p / (2.0f * M_PI) * asinf(max / a);
		}

		t = t - 1.0f;
		return -(a * powf(2.0f, 10.0f * t) * sinf((t * total - s) * (2.0f * M_PI) / p)) + min;
	}

	float ElasticOut(float t, float total, float min, float max)
	{
		max -= min;
		t /= total;

		float s = 1.70158f;
		float p = total * 0.3f;
		float a = max;

		if (t == 0.0f) return min;
		if (t == 1.0f) return min + max;

		if (a < fabsf(max))
		{
			a = max;
			s = p / 4.0f;
		}
		else
		{
			s = p / (2.0f * M_PI) * asinf(max / a);
		}

		return a * powf(2.0f, -10.0f * t) * sinf((t * total - s) * (2.0f * M_PI) / p);
	}

	float ElasticInOut(float t, float total, float min, float max)
	{
		max -= min;
		t /= total / 2.0f;

		float s = 1.70158f;
		float p = total * (0.3f * 1.5f);
		float a = max;

		if (t == 0.0f) return min;
		if (t == 2.0f) return min + max;

		if (a < fabsf(max))
		{
			a = max;
			s = p / 4.0f;
		}
		else
		{
			s = p / (2.0f * M_PI) * asinf(max / a);
		}

		if (t < 1.0f)
		{
			return -0.5f * (a * powf(2.0f, 10.0f * (t -= 1.0f)) * sinf((t * total - s) * (2.0f * M_PI) / p)) + min;
		}

		t = t - 1.0f;
		return a * powf(2.0f, -10.0f * t) * sinf((t * total - s) * (2.0f * M_PI) / p) * 0.5f + max + min;
	}

	float BackIn(float t, float total, float min, float max, float s)
	{
		max -= min;
		t /= total;
		return max * t * t * ((s + 1.0f) * t - s) + min;
	}

	float BackOut(float t, float total, float min, float max, float s)
	{
		max -= min;
		t = t / total - 1.0f;
		return max * (t * t * ((s + 1.0f) * t + s) + 1.0f) + min;
	}

	float BackInOut(float t, float total, float min, float max, float s)
	{
		max -= min;
		s *= 1.525f;
		t /= total / 2.0f;
		if (t < 1.0f) return max / 2.0f * (t * t * ((s + 1.0f) * t - s)) + min;

		t = t - 2.0f;
		return max / 2.0f * (t * t * ((s + 1.0f) * t + s) + 2.0f) + min;
	}

	float BounceIn(float t, float total, float min, float max)
	{
		max -= min;
		return max - BounceOut(total - t, total, 0.0f, max) + min;
	}

	float BounceOut(float t, float total, float min, float max)
	{
		max -= min;
		t /= total;

		if (t < 1.0f / 2.75f)
		{
			return max * (7.5625f * t * t) + min;
		}
		else if (t < 2.0f / 2.75f)
		{
			t -= 1.5f / 2.75f;
			return max * (7.5625f * t * t + 0.75f) + min;
		}
		else if (t < 2.5f / 2.75f)
		{
			t -= 2.25f / 2.75f;
			return max * (7.5625f * t * t + 0.9375f) + min;
		}
		else
		{
			t -= 2.625f / 2.75f;
			return max * (7.5625f * t * t + 0.984375f) + min;
		}
		return 0.0f;
	}

	float BounceInOut(float t, float total, float min, float max)
	{
		if (t < total / 2.0f)
		{
			return BounceIn(t * 2.0f, total, 0.0f, max - min) * 0.5f + min;
		}
		else
		{
			return BounceOut(t * 2.0f - total, total, 0.0f, max - min) * 0.5f + min + (max - min) * 0.5f;
		}
	}
}