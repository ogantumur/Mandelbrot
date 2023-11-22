#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixel_size = { pixelWidth, pixelHeight };
    m_aspectRatio = 1.0*pixelHeight / pixelWidth;
    m_plane_center = { 0,0 };
    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_state = State::CALCULATING;
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_vArray);
}

void ComplexPlane::zoomIn()
{
    m_zoomCount++;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { x, y };
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
    m_zoomCount--;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { x, y };
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
    stringstream sstr;

    sstr << "Mandelbrot Set\n";
    sstr << "Center(" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
    sstr << "Cursor(" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
    sstr << "Left-click to Zoom in\n";
    sstr << "Right-click to Zoom out";

    text.setString(sstr.str());
}

void ComplexPlane::updateRender()
{
    if (m_state == State::CALCULATING)
    {
        for (int i = 0; i < m_pixel_size.y; i++)
        {
            for (int j = 0; j < m_pixel_size.x; j++)
            {
                m_vArray[j + i * m_pixel_size.x].position = { (float)j,(float)i };
                Vector2i location(j, i);
                Vector2f coord = mapPixelToCoords(location);
                size_t iter = countIterations(coord);
                Uint8 r, g, b;
                iterationsToRGB(iter, r, g, b);
                m_vArray[j + i * m_pixel_size.x].color = { r,g,b };
            }
        }
        m_state = State::DISPLAYING;
    }
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
    size_t iterations = 0;
    Vector2f z = coord;

    while (iterations < MAX_ITER)
    {
        float x = z.x * z.x - z.y * z.y + coord.x;
        float y = 2.0 * z.x * z.y + coord.y;

        z = Vector2f(x, y);

        if (z.x * z.x + z.y * z.y > 4.0)
            break;
        iterations++;
    }
    return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    if (count == MAX_ITER)
    {
        r = 150;
        g = 0;
        b = 255;
    }
    else if (count >= 52 && count < MAX_ITER)
    {
        r = 100;
        g = 0;
        b = 255;
    }
    else if (count >= 38 && count < 52)
    {
        r = 0;
        g = 0;
        b = 255;
    }
    else if (count >= 26 && count < 38)
    {
        r = 0;
        g = 100;
        b = 255;
    }
    else if (count >= 12 && count < 26)
    {
        r = 0;
        g = 180;
        b = 255;
    }
    else if (count >= 0 && count < 12)
    {
        r = 0;
        g = 0;
        b = 0;
    }
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    float mappedX = ((mousePixel.x - 0) / static_cast<float>(m_pixel_size.x)) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0);
    float mappedY = ((mousePixel.y - m_pixel_size.y) / static_cast<float>(0 - m_pixel_size.y)) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0);

    return Vector2f(mappedX, mappedY);
}