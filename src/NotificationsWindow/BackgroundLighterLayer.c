//
// Created by Matej on 18.10.2015.
//
#ifdef PBL_COLOR
#include "BackgroundLighterLayer.h"
#include <pebble.h>

int getLuminance(GColor color)
{
    return 20 * color.r + 70 * color.g + 7 * color.b;
}

GColor getTextColor(GColor background)
{
    uint16_t luminance = getLuminance(background);
    return luminance > (MAX_LUMINANCE / 2) ? GColorBlack : GColorWhite;
}

void backgroud_lighter_layer_update(Layer* me, GContext* ctx)
{
    GBitmap* frameBuffer = graphics_capture_frame_buffer(ctx);

    uint8_t* pixelData = gbitmap_get_data(frameBuffer);

    GRect layerFrame = layer_get_frame(me);
    uint16_t x1 = layerFrame.origin.x;
    uint16_t y1 = layerFrame.origin.y;
    uint16_t x2 = x1 + layerFrame.size.w;
    uint16_t y2 = y1 + layerFrame.size.h;

    uint16_t screenWidth = gbitmap_get_bounds(frameBuffer).size.w;


    for (uint16_t y = y1; y < y2; y++)
    {
        for (uint16_t x = x1; x < x2; x++)
        {
            uint16_t arrayIndex = y * screenWidth + x;

            GColor curPixel = (GColor8) {.argb = pixelData[arrayIndex]};

            //Increase luminance of the pixel until it is bright enough to not make text in front unreadable
            while (getLuminance(curPixel) < MAX_LUMINANCE * 2 / 5)
            {
                if (curPixel.r < 3)
                    curPixel.r++;
                if (curPixel.g < 3)
                    curPixel.g++;
                if (curPixel.b < 3)
                    curPixel.b++;
            }

            pixelData[arrayIndex] = curPixel.argb;
        }
    }

    graphics_release_frame_buffer(ctx, frameBuffer);
}
#endif