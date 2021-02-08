#pragma once

// isolate each color part, divide by 2, add back in
#define DARKEN(color) ( \
	/*red*/   ( (  color        & 0xff ) / 2)        | \
	/*green*/ ((( (color >> 8 ) & 0xff ) / 2) << 8 ) | \
	/*blue*/  ((( (color >> 16) & 0xff ) / 2) << 16)   \
)
