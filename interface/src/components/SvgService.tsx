import internal from "stream";

export interface Dimensions {
    x: number;
    y: number;
}

export interface DimensionsDataController extends Dimensions {
  yActual?: number;
  xActual?: number;
}
  
export class SvgService {
    static loadContent(svgContent: string, svgContainer: HTMLElement){
      let svgService = new SvgService(svgContainer);
      svgService.loadXML(svgContent);
      return svgService;
    }

    static async load(file: File, svgContainer: HTMLElement) {
      if (!file) {
        return;
      }
  
      if (file.type !== "image/svg+xml") {
        throw new Error("wrong file type or no file provided");
      }
  
      let fileReader = new FileReader();
  
      return await new Promise<SvgService>((resolve, reject) => {
        fileReader.onloadend = () => {
          let result = fileReader.result as string;
          let svgService = SvgService.loadContent(result, svgContainer);
          resolve(svgService);
        };
  
        fileReader.onabort = e => {
          reject(e);
        };
  
        fileReader.onerror = e => {
          reject(e);
        };
  
        fileReader.readAsText(file);
      });
    }
  
    private path: SVGPathElement | undefined;
    private bBox: {width: number, height: number, x: number, y:number} | undefined;
    private circle: SVGCircleElement | undefined;
    private stepSize: number | undefined;
    private precision: number = 1000;
    private scale: Dimensions = { x: 1, y: 1 };
    private scaleRatio: Dimensions = { x: 1, y: 1 };
  
    constructor(private svgContainer: Element) {}
  
    public maxMoment = 0;
    public moment = 0;
    public scaledPoint: Dimensions | undefined;
  
    configure(scaleRatio: Dimensions, precision: number | undefined | null) {
      if (precision) {
        this.precision = precision;
      }
  
      if (
        this.scaleRatio.x !== scaleRatio.x ||
        this.scaleRatio.y !== scaleRatio.y
      ) {
        if (!this.bBox) {
          throw new Error("File not loaded yet!");
        }
  
        let bBox = this.bBox;
        let x = bBox.width;
        let y = bBox.height;
  
        let realRatio = scaleRatio.x / scaleRatio.y;
        let ratio = x / y;
        if (ratio > 1) {
          this.scale.x = 4000 / x;
          this.scale.y = this.scale.x / realRatio / ratio;
        } else {
          this.scale.y = 35000 / y;
          this.scale.x = this.scale.y * realRatio * ratio;
        }
  
        this.scaleRatio = scaleRatio;
      }
    }
  
    loadXML(data: string){
      let self = this;
  
      let container = self.svgContainer;
      container.innerHTML = data;
      let svg1 = container.getElementsByTagName("svg")[0];
      let path = svg1.getElementsByTagName("path")[0];
      let bBox = { width: +svg1.width.animVal.value, height: +svg1.height.animVal.value, x:0, y:0 };
  
      self.remoteAttributes(svg1);
      let viewBoxValue = " 0 0 " + bBox.width + " " + bBox.height;
      svg1.setAttribute("viewBox", viewBoxValue);
  
      // create a circle
      let circle = document.createElementNS(
        "http://www.w3.org/2000/svg",
        "circle"
      );
  
      let circleRatio = (bBox.width + bBox.height) / 100;
      circle.setAttribute("r", circleRatio.toString());
      circle.setAttribute("fill", "red");
  
      // attach it to the container
      svg1.appendChild(circle);
  
      self.circle = circle;
      self.path = path;
      self.stepSize = path.getTotalLength() / self.precision;
      self.maxMoment = path.getTotalLength();
      self.moment = 0;
      self.bBox = bBox;
    }

    onloadEnd(result: string) {
      let self = this;
  
      let container = self.svgContainer;
      container.innerHTML = result;
      let svg1 = container.getElementsByTagName("svg")[0];
      let path = svg1.getElementsByTagName("path")[0];
      let bBox = svg1.getBBox();
  
      self.remoteAttributes(svg1);
      let viewBoxValue = " 0 0 " + bBox.width + " " + bBox.height;
      svg1.setAttribute("viewBox", viewBoxValue);
  
      // create a circle
      let circle = document.createElementNS(
        "http://www.w3.org/2000/svg",
        "circle"
      );
  
      let circleRatio = (bBox.width + bBox.height) / 100;
      circle.setAttribute("r", circleRatio.toString());
      circle.setAttribute("fill", "red");
  
      // attach it to the container
      svg1.appendChild(circle);
  
      self.circle = circle;
      self.path = path;
      self.stepSize = path.getTotalLength() / self.precision;
      self.maxMoment = path.getTotalLength();
      self.moment = 0;
      self.bBox = bBox;
    }
  
    moveNext() {
      let self = this;
      let path = self.path;
      let bBox = self.bBox;
      if (!path) throw new Error("No path loaded yet !");
      if (!bBox) throw new Error("No bBox loaded yet !");
      if (!self.stepSize) throw new Error("No stepSize loaded yet !");
      if (!self.circle) throw new Error("No circle loaded yet !");
  
      let first = true;
      let pos = path.getPointAtLength(self.moment);
      let iterations = 0;
      while (
        (first ||
          pos.x >= bBox.x + bBox.width ||
          pos.y > bBox.y + bBox.height) &&
        self.moment < self.maxMoment
      ) {
        first = false;
        self.moment += self.stepSize;
        pos = path.getPointAtLength(self.moment);
        iterations++;
      }
  
      self.circle.setAttribute("cx", pos.x.toString());
      self.circle.setAttribute("cy", pos.y.toString());
      self.scaledPoint = self.getScaledPoint(pos);
  
      return iterations;
    }
  
    getScaledPoint(point: Dimensions) {
      let self = this;

      if (!self.bBox) throw new Error("No bBox loaded yet !");

      let scaledPoint = { x: point.x, y: point.y };
      scaledPoint.x -= self.bBox.x;
      scaledPoint.y -= self.bBox.y;
  
      scaledPoint.x *= self.scale.x;
      scaledPoint.y *= self.scale.y;
  
      scaledPoint.x = Math.round(scaledPoint.x);
      scaledPoint.y = Math.round(scaledPoint.y);
      return scaledPoint;
    }
  
    private remoteAttributes(element: SVGSVGElement) {
      for (var i = element.attributes.length - 1; i >= 0; i--) {
        element.removeAttribute(element.attributes[i].name);
      }
    }
}  