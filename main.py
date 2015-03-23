import Forwarder
import Retriever

if __name__ == "__main__":
    port = 1234
    fake = True
    txcan = Forwarder.ForwardCAN(port, fake)
    rxcan = Retriever.RetrieveCAN()
    txcan.send_can(rxcan)
