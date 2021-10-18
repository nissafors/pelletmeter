/* Copyright (C) 2021 Andreas Andersson */

#ifndef REQUEST_H
#define REQUEST_H

/**
 * Create an HTTP POST request message to post a short JSON snippet for the pelletmeter server:
 * {
 *   cm: <measurement>
 * }
 * 
 * @param measurement   The pelletmeter distance measurement in cm.
 *
 * @return Pointer to the full HTTP POST request text.
 */
char* createPostRequest(const int measurement, const char* host, const int port);

#endif // REQUEST_H