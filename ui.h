#ifndef UI_H
#define UI_H

/* Returns users terminal to
 * its original configuration.
 * Function should be attached to run
 * on exit.
 */
void disable_raw_mode();

/* Change input to not be buffered
 * by new line. All input is passed
 * to stdin immediately.
 */
void enable_raw_mode();


#endif /* UI_H */

