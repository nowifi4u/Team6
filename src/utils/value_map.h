#pragma once

template <typename Ty>
struct ValueMap {

	Ty input_low, input_high, output_low, output_high;

	void set_input(Ty input_low, Ty input_high)
	{
		this->input_low = input_low;
		this->input_high = input_high;
	}

	void set_output(Ty output_low, Ty output_high)
	{
		this->output_low = output_low;
		this->output_high = output_high;
	}

	void set(Ty input_low, Ty input_high, Ty output_low, Ty output_high)
	{
		this->input_low = input_low;
		this->input_high = input_high;
		this->output_low = output_low;
		this->output_high = output_high;
	}

	ValueMap(Ty input_low, Ty input_high, Ty output_low, Ty output_high)
		: input_low(input_low), input_high(input_high), output_low(output_low), output_high(output_high) { }

	ValueMap(Ty input_low, Ty input_high)
		: ValueMap(input_low, input_high, 0, 1) { }

	[[NODISCARD]]
	constexpr Ty map(Ty input) const
	{
		return ValueMap::map(input, input_low, input_high, output_low, output_high);
	}

	[[NODISCARD]]
	constexpr Ty get_scale() const
	{
		return (output_high - output_low) / (input_high - input_low);
	}

	[[NODISCARD]]
	static constexpr Ty map_scale(Ty input, Ty input_low, Ty input_high)
	{
		const Ty input_size = input_high - input_low;
		const Ty input_delta = input - input_low;

		return input_delta / input_size;
	}

	[[NODISCARD]]
	static constexpr Ty map_unscale(Ty scale, Ty output_low, Ty output_high)
	{
		const Ty output_size = output_high - output_low;
		const Ty output_delta = output_size * scale;

		return output_low + output_delta;
	}

	[[NODISCARD]]
	static constexpr Ty map(Ty input, Ty input_low, Ty input_high, Ty output_low, Ty output_high) noexcept
	{
		const Ty scale = map_scale(input, input_low, input_high);

		return map_unscale(scale, output_low, output_high);
	}

}; // class ValueMap
