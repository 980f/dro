#ifndef UART_H
#define UART_H

namespace SAM {
/** the singluar uart, not to be confused with the USarts.
 * It is said to be a pure subset of the USart.
 * This variant does not admit of the possibility of DMA. We'll burn that bridge when we come to it.
*/
class Uart{

public:
    /** the isr. a stub is locally generated to vector to it */
    void isr(void)const;
private:
    /** called by isr on an input event.
     * negative values of @param are notifications of line errors, -1 for interrupts disabled */
    typedef bool (*Receiver)(int incoming);
    Receiver receive;
    /** called by isr when transmission becomes possible.
     *  @return either an 8 bit unsigned character, or -1 to disable transmission events*/
    typedef int (*Sender)();
    Sender send;
protected:
    /** read at least one char from fifo, stop on error. @param LSRValue is recent read of line status register, @returns final read of line status register */
//    unsigned tryInput(unsigned LSRValue)const;
public:
    //trying for full const'd instance
    Uart(Receiver receiver,Sender sender);

    /** @param hertz is used to compute multiple register settings to get the closest to the desired value.
     *  @returns the actual rate.@param sysFreq if left 0 uses the running clock's frequency for calculations else the given value is used.  */
    unsigned setBaud(unsigned hertz, unsigned sysFreq = 0) const;
    /** @param coded e.g. "8N1" for typical binary protocol, 8E2 for modbus. The SAM Uart only supports 8?1 */
    bool setFraming(const char *coded) const;

    /** called to request that transmit isr be enabled or disabled.
     * Note: if enabled and wasn't enabled then send() will immediately be called to get first byte to send, IE send() isn't only called from isr (unless we can arrange for that!). */
    void beTransmitting(bool enabled = true)const;
    /** call to request that reception isr be enabled or disabled. */
    void reception(bool enabled = true)const;
    /** set send function. @returns this. */
    Uart &setTransmitter(Sender sender);
    /** set reception routine. . @returns this. */
    Uart &setReceiver(Receiver receiver);
    /** indirect access to the nvic interrupt enable */
    void irq(bool enabled)const;
//    void initializeInternals()const;
public:
    void setLoopback(unsigned mode);
};

};
#endif // UART_H
