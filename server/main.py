import argparse
import hyassetserver


def main() -> int:
    parser = argparse.ArgumentParser(description="Hydrogen Asset Server")
    parser.add_argument("dll", type=str, help="path to the HydrogenServer_Wrapper .dll/.so/.dylib")
    parser.add_argument("--host", type=str, help="host address")
    parser.add_argument("--port", type=int, help="host port")
    args = parser.parse_args()

    server = hyassetserver.HyAssetServer(args.dll)
    server.run(host=args.host, port=args.port)
    return 0


if __name__ == "__main__":
    exit(main())
